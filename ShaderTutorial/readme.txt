컴퓨터 그래픽스 실습(1) PA1

깨달은 점과 중요한 점 정리

1. 변환 행렬은 신축 + 회전 + 이동 순으로 더해줘야한다.

2. 여러개의 모델을 렌더링 할 때는 한 모델의 모델 렌더링과 셰이더 렌더링을 해준 뒤에 다음 모델의 모델 렌더링과 셰이더 렌더링을 해줘야 한다. 모델 렌더링과 셰이더 렌더링을 한꺼번에 호출한다면, 젤 마지막에 모델 렌더링이 된 모델의 정보로 전부 셰이더 렌더링이된다.

3. Initialize 관련 함수들은 1번만 호출되기 때문에 실시간으로 Update 해줘야 하는 부분들은 Frame()안에 넣어야 반영된다.

4. cbuffer를 통해 셰이더 파일과 정보를 주고 받을 수 있으며, 똑같은 버퍼를 CPU쪽에 만들어서 GPU 쪽으로 보내줘야한다.

5. Raster State의 개념을 이용하여 다양한 커스텀 State를 만들 수 있다.

6. 디버깅 메세지를 확인하고, return false가 되는 시점을 하나하나 디버깅 하여 문제점을 찾은 다음 수정을 하니, 오류난 부분을 확인하기가 정말 편했다.

7. pixel shader에 float brightnessNum 하나 있는 cbuffer를 만들어 GPU 쪽에 버퍼를 할당 해주려고 했지만, 셰이더를 초기화 할수 없다는 오류 메세지가 반복되어 디버깅을 한 결과 vertex shader에 있는 cbuffer를 위해 만들어진 버퍼는 192 byte 였고, 내가 만든 cbuffer는 4byte 였다. GPU는 16 byte 단위로 처리를 한다는 수업 내용이 떠올라 버퍼 크기에 곱하기 4를 해줬더니 정상적으로 버퍼가 생성되어 셰이더 초기화가 완료 되었다.

8. fill mode를 변경하는 과정에서 처음엔 D3D Class의 Begin Scene에 넣었고, 잘 실행 되었지만 오류 발생시 result를 false로 return하는 과정을 처리할 수 없기에 Graphics 클래스에 UpdateFillMode라는 bool 자료형 함수를 만들어 Frame()함수에 넣어서 예외처리도 가능하다.