#!/bin/bash
set -e

echo "=============================================="
echo " 🔐 GitHub SSH Key Setup Wizard"
echo "=============================================="

# 1️⃣ 기본 정보
read -p "👉 GitHub 계정 이메일을 입력하세요: " EMAIL
KEY_PATH="$HOME/.ssh/id_ed25519"

# 2️⃣ 기존 키 확인
if [ -f "$KEY_PATH" ]; then
    echo "⚠️ 이미 SSH 키가 존재합니다: $KEY_PATH"
    read -p "덮어쓸까요? (y/N): " yn
    if [[ "$yn" =~ ^[Yy]$ ]]; then
        rm -f "$KEY_PATH" "$KEY_PATH.pub"
    else
        echo "➡️ 기존 키를 사용합니다."
    fi
fi

# 3️⃣ SSH 키 생성
if [ ! -f "$KEY_PATH" ]; then
    echo "🔧 새 SSH 키를 생성합니다..."
    ssh-keygen -t ed25519 -C "$EMAIL" -f "$KEY_PATH" -N ""
else
    echo "✅ 기존 키를 사용합니다."
fi

# 4️⃣ SSH 에이전트 시작 및 키 등록
echo "🚀 SSH 에이전트를 시작하고 키를 추가합니다..."
eval "$(ssh-agent -s)" >/dev/null
ssh-add "$KEY_PATH"

# 5️⃣ 공개키 출력 및 등록 안내
PUB_KEY=$(cat "$KEY_PATH.pub")
echo
echo "=============================================="
echo "📋 아래 공개키를 GitHub에 등록하세요."
echo "➡️  GitHub → Settings → SSH and GPG keys → New SSH key"
echo
echo "$PUB_KEY"
echo
echo "🔗 바로 열기: https://github.com/settings/keys"
echo "=============================================="
echo

# 6️⃣ Git 원격 주소를 SSH로 변경
REMOTE_URL=$(git remote get-url origin 2>/dev/null || echo "")
if [[ "$REMOTE_URL" == *"https://"* ]]; then
    USERNAME=$(echo "$REMOTE_URL" | sed -E 's#https://github.com/([^/]+)/.*#\1#')
    REPO=$(basename -s .git "$REMOTE_URL")
    NEW_URL="git@github.com:${USERNAME}/${REPO}.git"
    git remote set-url origin "$NEW_URL"
    echo "✅ Git 원격 주소를 SSH 형식으로 변경했습니다."
    echo "   $NEW_URL"
else
    echo "ℹ️ Git 원격 저장소가 HTTPS가 아니거나 설정되어 있지 않습니다."
    echo "   필요 시 수동으로:"
    echo "   git remote set-url origin git@github.com:<USERNAME>/<REPO>.git"
fi

# 7️⃣ 테스트 안내
echo
echo "=============================================="
echo "✨ SSH 키 설정 완료!"
echo "다음 명령으로 GitHub 연결을 테스트하세요:"
echo
echo "  ssh -T git@github.com"
echo
echo "정상이라면 아래 메시지가 뜹니다:"
echo "  'Hi <username>! You've successfully authenticated...'"
echo
echo "이후 git push 시 비밀번호 입력 없이 자동으로 인증됩니다."
echo "=============================================="

