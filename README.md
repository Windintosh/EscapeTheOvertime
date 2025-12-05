# 🏃‍♂️ 칼퇴의 정석 (Escape the Overtime)

> **"금요일 오후 6시 1분. 주말을 즐기려는 김 대리와 야근을 시키려는 부장님의 숨 막히는 추격전!"**

![Unreal Engine 5](https://img.shields.io/badge/Unreal_Engine-5.x-black?logo=unrealengine)
![Language](https://img.shields.io/badge/Language-C%2B%2B_%26_BluePrint-blue?logo=cplusplus)
![Genre](https://img.shields.io/badge/Genre-Stealth_Action-red)
![Team](https://img.shields.io/badge/Team-5_People-green)

## 📂 프로젝트 소개
**'칼퇴의 정석'**은 언리얼 엔진 5를 활용한 3인칭 오피스 잠입 액션 게임입니다.
단순한 추격전을 넘어, **고도화된 AI 시스템(EQS, Perception)**과 **비헤이비어 트리(Behavior Tree)**를 심도 있게 연구하여 긴장감 넘치는 잠입 플레이를 구현하는 데 중점을 두었습니다.

플레이어는 '김 대리'가 되어, 시각과 청각을 곤두세운 '부장님(AI)'의 감시망을 뚫고 엘리베이터로 탈출해야 합니다.

🔗 **[👉 칼퇴의 정석 개발 노션(Notion) 바로가기](https://wax-passbook-93d.notion.site/2bfe919330e380379d77e74a150b67f4?pvs=74)**
*(기획서, WBS, 기술 명세서 및 상세 개발 로그 확인 가능)*

---

## 🎮 주요 기능 (Key Mechanics)

### 1. 플레이어 (김 대리)
* **잠입 액션 (Stealth):** `Ctrl` 키를 사용하여 파티션 뒤나 책상 밑으로 은폐(Crouch) 가능.
* **소음 시스템 (Noise Generation):**
    * 걷기: 소음 없음 (안전)
    * 달리기 (`Shift`): 소음 발생 → AI의 청각 자극 및 추격 유발
* **전략적 상호작용:** 전화기나 복사기를 작동시켜 소음을 유발, 부장님을 유인하고 따돌림.
* **스태미나 관리:** 전력 질주에 제한을 두어 전략적인 이동 경로 설계 필요.

### 2. 적 AI (부장님)
* **감각 시스템 (AI Perception):**
    * **시각(Sight):** 전방 90도 시야각 구현. 플레이어 식별 시 즉시 추격 모드 전환.
    * **청각(Hearing):** 발소리나 사물 소음 감지 시 해당 위치(`LastHeardLocation`)로 의심 이동.
* **지능적 수색 (EQS):** 플레이어를 놓쳤을 때 단순히 제자리로 돌아가지 않고, **주변의 은폐 가능한 지점(책상 밑, 사각지대)**을 우선적으로 수색하는 고지능 로직 구현.
* **행동 패턴:** `[업무]` ↔ `[순찰]` ↔ `[의심]` ↔ `[추격]` ↔ `[휴식(커피)]`의 유기적 상태 전환.

---

## 🛠️ 핵심 기술 연구 (R&D Highlights)
이 프로젝트는 단순 기능 구현을 넘어, 언리얼 엔진의 AI 시스템을 깊이 있게 연구하고 적용했습니다.

### 🧠 1. Advanced AI Logic
* **Behavior Tree & Blackboard:** 상태 패턴을 트리 구조로 시각화 및 설계하여 복잡한 AI 행동을 모듈화.
* **Decorator & Observer Aborts:** 순찰 중이더라도 청각/시각 자극이 들어오면 즉시 현재 행동을 중단(Abort)하고 반응하도록 우선순위 로직 설계.
* **Smart Objects:** AI의 피로도 변수에 따라 탕비실 커피 머신과 상호작용하는 환경 인지 로직 구현.

### 🎯 2. Environment Query System (EQS)
* 단순한 랜덤 이동(`GetRandomPoint`)의 한계를 극복하기 위해 EQS를 도입.
* **Context 기반 가중치 부여:** AI 기준으로 '플레이어가 숨기 좋은 위치(Visiblity Trace 검사)'에 높은 점수를 부여하여, 인간처럼 구석진 곳을 찾아보는 수색 알고리즘 적용.

### 🔊 3. Noise Tagging System
* `Make Noise` 함수와 `Gameplay Tag`를 결합하여 소음의 종류를 구분.
* 부장님 AI가 '배경 소음'은 무시하고, '플레이어 유발 소음(발소리, 전화기)'에만 반응하도록 필터링 구현.

---

## 📸 인게임 스크린샷 (In-Game)

| 잠입 (Stealth) | AI 시야 디버그 (Debug) |
|:---:|:---:|
| <img src="이미지경로1" width="100%" alt="파티션 뒤에 숨은 김 대리"> | <img src="이미지경로2" width="100%" alt="EQS 수색 중인 부장님"> |
| *파티션 뒤에 숨은 김 대리* | *EQS로 수색 지점을 찾는 부장님* |

---

## 👨‍💻 개발팀 (Developers)
* **Lead Programer / AI:** [박재홍] - 비헤이비어 트리 아키텍처, EQS 설계, 캐릭터 이동 시스템, 상호작용 로직
* * **Tech Artist:** [유상우] - 애니메이션 리타겟팅, 연출 및 이펙트, 사무실 레벨 디자인, NavMesh 최적화
* **Project Manager:** [정윤성] - 기획 및 문서관리, UI, 에셋 (스마트 오브젝트, 사운드, 소음 시스템)

---
Copyright 2025. Escape The Overtime Team. All rights reserved.
