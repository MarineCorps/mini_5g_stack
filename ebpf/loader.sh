#!/bin/bash
set -e

# --- 설정 ---
# 컴파일한 커널 소스의 정확한 경로
KSRC=/src/WSL2-Linux-Kernel-linux-msft-wsl-6.6.87.2

# --- 1단계: 환경 준비 ---
echo "Step 1/5: Preparing environment..."
# BPF 파일 시스템 마운트
if [ -d /sys/fs/bpf ] && ! mount | grep -q '/sys/fs/bpf type bpf'; then
    sudo mount -t bpf bpf /sys/fs/bpf/
fi
# vmlinux.h 생성
bpftool btf dump file ${KSRC}/vmlinux format c > vmlinux.h

# --- 2단계: eBPF 프로그램 컴파일 ---
echo "Step 2/5: Compiling eBPF C code..."
clang -O2 -g -target bpf -D__TARGET_ARCH_x86 -c bpf_prog.c -o bpf_prog.o

# --- 3단계: 기존 eBPF 객체 정리 ---
echo "Step 3/5: Cleaning up old pinned files..."
sudo rm -f /sys/fs/bpf/janus_qfi_counter
sudo rm -f /sys/fs/bpf/janus_sendmsg_hook

# --- 4단계: eBPF 프로그램 및 맵 로드 ---
echo "Step 4/5: Loading eBPF program and map..."
# 프로그램과 맵을 한번에 로드하고 pin
sudo bpftool prog load bpf_prog.o /sys/fs/bpf/janus_sendmsg_hook map name qfi_count_map pin /sys/fs/bpf/janus_qfi_counter

# --- 5단계: Kprobe 부착 ---
echo "Step 5/5: Attaching program to kprobe..."
# 로드된 프로그램을 kprobe에 attach
sudo bpftool prog attach pinned /sys/fs/bpf/janus_sendmsg_hook kprobe/__x64_sys_sendmsg || \
sudo bpftool prog attach pinned /sys/fs/bpf/janus_sendmsg_hook kprobe/sys_sendmsg
echo "---"
echo "✅ Setup complete. All steps succeeded."