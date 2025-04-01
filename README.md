# 🎮 멀티플레이어 숫자 야구 게임 (Unreal Engine 5)

1~9까지 겹치지 않는 세 자리 숫자를 맞추는 턴제 멀티플레이어 게임입니다.  
Unreal Engine 5.3 + C++ 기반으로 Listen Server 방식의 2인용 게임입니다.

---

## 🔧 기술 스택

- Unreal Engine 5.3
- C++
- Blueprint (하이브리드 방식)
- Listen Server (멀티플레이어)

---

## 🎯 게임 룰

- 서버는 시작 시 3자리 중복 없는 숫자(1~9)를 생성합니다.
- Host와 Guest는 차례대로 `/123` 같은 숫자를 입력해 정답을 추측합니다.
- 판정은 다음과 같이 처리됩니다:
  - **스트라이크(S)**: 자리수와 숫자가 모두 맞음
  - **볼(B)**: 숫자는 맞지만 자리는 다름
  - **아웃**: 맞는 숫자가 하나도 없음

- 각 플레이어는 최대 3번의 기회를 가집니다.
- 누가 먼저 맞추는지에 따라 승패가 갈리며, 비기거나 모두 실패하면 무승부가 됩니다.

---

## 📂 주요 클래스 역할

### `ABaseballGameModeBase`
- 전체 게임 흐름 제어
- Ready 체크, StartGame, StartTurn, EndTurn 관리
- 난수 생성 및 승패 판정

### `ABaseballGameStateBase`
- 게임 상태 변수 동기화
- 현재 턴, 시도 횟수, 결과, 정답 등 관리
- `OnRep_` 함수를 통해 UI 갱신

### `ABaseballGameController`
- 위젯 표시 및 입력 전달
- Ready 버튼, 숫자 입력, 결과 출력 처리
- `Client_StartTurn`, `Client_ShowGuessResult` 등의 RPC 사용

### `UGameWidget`
- Ready 버튼, 입력창, 결과 보드, 시작/종료 알림 UI 구성
- 버튼 클릭 시 Delegate를 통해 Controller에 전달

---

## ⚙️ 주요 기능

- 턴 기반 멀티플레이어 진행
- 30초 제한 시간 내에 추측하지 않으면 자동 Out
- 정답 비교 (S/B/OUT 판정)
- 클라이언트마다 자기 결과 보드 표시
- 승리/무승부/패배 시 결과 및 정답 표시
- 게임 자동 재시작 기능

---

## 📅 작업 정리 (2025.03.25 ~ 2025.03.29)

### ✅ 3월 25일 (월)
- 프로젝트 설계 구조 잡기 (GameMode, GameState, Controller 분리)
- 숫자 판정 로직 `JudgeGuess` 함수 설계
- 난수 생성 함수 구현

### ✅ 3월 26일 (화)
- Replication 시스템 이해
  - `UPROPERTY(Replicated)`와 `OnRep_` 함수
  - `GetLifetimeReplicatedProps` 오버라이드
- `Server_` / `Client_` 함수 개념 학습
- `UserID`를 이용한 Host / Guest 구분

### ✅ 3월 27일 (수)
- Delegate vs DynamicDelegate 비교
- 위젯과 Controller 연결 구조 정리
- 게임 흐름 완성 (StartGame, StartTurn, EndTurn, CheckGameResult)
- GameState의 상태 변수 정리 (Tries, ClearTurn 등)
- 정답을 나타내는 보드 텍스트 추가

### ✅ 3월 28~29일 (목~금)
- EndTurn과 승패 조건 정리
- 동점 및 턴 동기화 문제 해결
- 게임 결과 출력 및 UI 표시 최적화
- Guess / Result Text 분리
- 타이머 UI 추가 및 클라이언트에서도 시간 표시

---

## 🧠 배운 점 정리

- `Replication`, `OnRep`, `RPC`의 흐름
- `GetWorld()`와 `PlayerControllerIterator`로 모든 플레이어 접근
- `Delegate`, `DynamicDelegate`, `BindUObject`, `BindUFunction` 차이점
- 서버/클라이언트 권한 처리 (`HasAuthority()`)
- UMG 위젯 동적 생성 및 바인딩 처리
- 게임 상태 관리와 UI 연동 (클라이언트에 맞게 UI 분기)

---

## 💻 실행 방법

1. `.uproject` 열기
2. Visual Studio에서 `Ctrl + B`로 전체 빌드
3. 에디터에서 `Selected Viewport` 모드로 실행 (Simulate 금지)
4. "Listen Server" 옵션을 켜고 두 인스턴스를 실행

---

## 📌 주의사항

- 반드시 **2인 플레이** 필요
- GameMode는 `GameModeBase`, GameState는 `GameStateBase` 상속 사용
- Ready 버튼이 준비 상태를 알려줌

---

## 🙌 만든 사람

- 제작자: (너의 이름)
- 작성일: 2025.03.29

