# Final Project
# Avengers 3D Scene
### 어벤져스 영화의 유명한 전투 장면을 DirectX를 사용하여 제작한 3D 씬입니다.

<div aling="center">
  <img src="https://github.com/Chanwoongs/ComputerGraphicsPractice1/assets/26241243/4b95ee35-ed1a-4638-956e-ad4832944a40" width="854" height="480"/>    
</div>

### [YouTube 영상](https://www.youtube.com/watch?v=AvMTiAA5sag)
### [Project Folder](https://github.com/Chanwoongs/ComputerGraphicsPractice1/tree/main/ProjectAssignment4)

### 개발 기간 (1인 개발)
> #### 2022.05 ~ 2022.06

### 개요
> #### 그래픽스 이론을 적용하여 구현하기 위해 만든 가상 Scene입니다.

### 기술
> #### [3D 오브젝트 인스턴싱](https://github.com/Chanwoongs/ComputerGraphicsPractice1/blob/main/ProjectAssignment4/Project%20Assignment%204/modelclass.cpp)
> 규칙적이고 많은 3D Object를 배치하기 위해 인스턴싱 사용, CPU 및 GPU의 부하를 줄이고 렌더링 시 성능 향상
> #### [HLSL, Point Light, Fog shader](https://github.com/Chanwoongs/ComputerGraphicsPractice1/blob/main/ProjectAssignment4/Project%20Assignment%204/data/light.ps)
> Texture, Ambient, Diffuse, Specular Lighting, Point Light, Fog shader HLSL 프로그래밍을 통해 pixel 값을 조절하여 적용
> #### 카메라 및 오브젝트 이동 회전
> #### 빌보드 기법을 활용한 Particle System

### 프로젝트를 통해 얻은 점
#### 그래픽스 지식 구현
> 이론으로 배웠던 렌더링 파이프라인을 구축하고 color, texture, light, font, fog shader를 하나씩 구현해가며 화면에 물체가 보여지는 과정을 익힘.
#### 최적화의 중요성
> 인스턴싱을 적용하지 않고 많은 모델을 띄웠을 땐 카메라 이동이 거의 되지 않았다. 인스턴싱 적용 후엔 렉이 많이 사라졌다.

### 아쉬운 점
> #### 긴 오브젝트 로딩 시간
> 화면에 띄울 오브젝트의 종류가 다양하다 보니 컴파일 후 로딩에 시간이 많이 걸렸다. 이를 멀티 쓰레딩을 활용하면 시간을 단축 시킬 수 있을 것 같다.
> #### 모두 같은 Material의 느낌
> 현재 모든 3D Object가 같은 Shader를 사용하다 보니 Object Material의 질감이 다 같게 표현되어 있다. 이를 모델마다 다른 쉐이더를 적용해 Matte 질감과 Glossy 질감을 구분할 수 있도록 해야겠다.
