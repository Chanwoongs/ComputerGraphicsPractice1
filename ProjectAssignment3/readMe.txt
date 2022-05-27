PA3 기술 문서

1번
Graphics class에서 모델을 생성할 때 생성자에 instancing할 모델들의 위치를 배열로 넘겨주고, instanceCount도 넘겨주어 Model class에서 이 정보를 가지고 Initialize를 할 수 있도록 설계하였습니다. 배열과 배열의 크기를 넘겨주었기 때문에 정보를 간단하게 for문으로 자동으로 넘겨 받을 수 있도록 하였습니다.

2번
Direct Input을 사용하여 키입력을 받도록 하였으며, 키를 누를시 단순히 Light의 Power를 줄이는 방법이 아닌 Pixel Shader에서 각각 Light 들이 Light의 Toggle(0 또는 1) 값에 따라 계산을 하거나 계산을 하지 않거나 하는 방법을 사용하여 빛을 끄고 킬수 있습니다.

3번
Direct Input을 사용하여 키입력과 마우스 좌표 값을 받아왔으며, UpdateCamera()와 DirectInput()함수를 만들지 않고 카메라의 Render() 함수에 UpdateCamera()의 기능을 합하였습니다. E키를 누르면 상승하고 Q키를 누르면 하강하는 기능도 up벡터를 수정해주어 간단하게 넣어봤습니다.