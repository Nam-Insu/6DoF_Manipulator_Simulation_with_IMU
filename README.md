# 작품 제목

광운대학교 로봇학부 학술소모임 **'BARAM'** 20년도 후반기 6DoF_Actuator_Simulation에 대한 소스코드입니다.  

## 개발 환경
|OS|사용 언어|사용 IDE|
|:---:|:---:|:---:|
| Ubuntu 18.04 | C++ | Qt Creator, Ros Melodic|

## 프로젝트 개발 동기

-  현재 많은 Robot들에 Manipulator가 사용되고 있습니다. Robot의 동작을 위해서는 Manipulator가 거의 필수라고 봐도 무방할 정도로 많이 사용 중인데, 이러한 Manipulator들을 연구 및 사용하기 위해서는 기구학 해석은 없으면 안되는 필수 요소입니다. 
-  이에 저는 정기구학을 공부해보고 이를 사용해보고자, 시뮬레이션 툴인 Coppeliasim의 V-rep에서 6-DOF Manipulator 을 생성한 후 IMU값에 따라 End Effector의 위치 조정을 구현해보려고 합니다.

## 프로젝트 개요
1. 로봇팔의 Inverse Kinematics 계산
>- End Effector와 Base 간의 Jacobian 행렬 연산
2.  Simulation 환경 구축
>- ROS Api를 활용하여 Coppeliasim 외부의 input으로 Actuator의 joint 각도변환
3. IMU의 3-Axis Linear Velocity, 3-Axis Angular Velocity 값 Parsing
4. USER의 모션에 따라 simulation 내의 End Effector 위치 변환
>- 파싱한 IMU 데이터 값으로 역기구학을 계산한다. 계산된 Output
 Coppeliasim내 로봇팔 Joint의 target Angle로 설정한다.


## System Architecture
<p align="center"><img src="https://user-images.githubusercontent.com/54669783/109418307-132edf80-7a0b-11eb-9c24-2e49965c7712.png" width="600px"></p>  


### Code Overview  
- IMU 값을 받아 Pasing하는 Code
- Actuator의 기구학을 풀어 Jacobian 행렬을 구성해 Joint 각도를 구하는 Code
- 각 Node간의 통신을 위한 msg code
- CoppeliaSim과 ROS를 연동하여 Actuator의 Joint 값을 Update 해주는 Code

### Project scenario

1. IMU를 UART_Cable로 연결하여 Data를 Pasing하여 가져온다.
2. CoppeliaSim을 가동하여 Actuator의 초기 Position에 둔다.
3. IMU의 위치가 변화함에 따라 CoppeliaSim내 Actuator End - Effector가 변하는 것을 확인한다



## 프로젝트 결과

<p align="center"><img src="https://user-images.githubusercontent.com/54669783/109418321-29d53680-7a0b-11eb-9a9b-acc0c8d8b0eb.png" width="500px"></p>  
<p align="center"> 초기값을 설정한 기본 Pose </p>  

<p align="center"><img src="https://user-images.githubusercontent.com/54669783/109418322-2cd02700-7a0b-11eb-985f-fe71b8a4521f.png" width="500px"></p>  
<p align="center"> Target Position을 변경한 후 이동한 모습 </p>  

<p align="center"><img src="https://user-images.githubusercontent.com/54669783/109418317-22ae2880-7a0b-11eb-927b-6955dcd82d8c.gif" width="500px"></p>  
<p align="center"> IMU를 이용해 Target Position이 변해 End-Effector가 이동하는 모습 </p>  

