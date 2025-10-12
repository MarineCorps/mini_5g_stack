#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

struct user_iovec {
    __u64 iov_base;
    __u64 iov_len;
};

struct user_msghdr {
    __u64 msg_name;
    __u32 msg_namelen;
    __u64 msg_iov;
    __u64 msg_iovlen;
    __u64 msg_control;
    __u64 msg_controllen;
    __u32 msg_flags;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 64);
    __type(key, __u32);
    __type(value, __u64);
} qfi_count_map SEC(".maps");

SEC("kprobe/__x64_sys_sendmsg")
int BPF_KPROBE(kprobe__sys_sendmsg, int fd, struct user_msghdr *msg_user, unsigned int flags)
{
    struct user_msghdr msg_local = {};
    struct user_iovec iov_local = {};
    unsigned char headers[4];
    __u32 qfi;
    __u64 *count, init_val = 1;

    // 1️⃣ user_msghdr 복사
    if (bpf_probe_read_user(&msg_local, sizeof(msg_local), msg_user))
        return 0;

    // 2️⃣ msg_iov 포인터 복사
    if (!msg_local.msg_iov)
        return 0;
    if (bpf_probe_read_user(&iov_local, sizeof(iov_local), (void*)msg_local.msg_iov))
        return 0;

    // 3️⃣ iov_base에서 데이터 읽기
    if (!iov_local.iov_base)
        return 0;
    if (bpf_probe_read_user(&headers, sizeof(headers), (void*)iov_local.iov_base))
        return 0;

    // 4️⃣ QFI 추출 및 카운트 증가
    qfi = headers[2];
    count = bpf_map_lookup_elem(&qfi_count_map, &qfi);
    if (count)
        __sync_fetch_and_add(count, 1);
    else
        bpf_map_update_elem(&qfi_count_map, &qfi, &init_val, BPF_ANY);

    bpf_printk("[JANUS eBPF] sys_sendmsg hooked! QFI=%u\n", qfi);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
