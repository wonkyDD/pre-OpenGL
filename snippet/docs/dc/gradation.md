# OpenGL 그라데이션 shader 만들기

<!-- https://gall.dcinside.com/mgallery/board/view?id=github&no=42459 -->
<!-- snippet/resources/gradation/gradation.mov -->

<br>
<br>

## 1. 서론

OpenGL을 입문하고서 만들어보고 싶었던<br>
shader를 만들 수 있게되어 글을 남기고자 합니다.

셰이더를 처음접한것은<br>
shadertoy의 템플릿에서였습니다.<br>
https://www.shadertoy.com/

엄청 어려울 줄 알았는데<br>
그래도 얼마없는 지식가지고도 만들 수 있다는게 신기했네요.

윈도우 라이브러리로는 `glfw`를 사용하였습니다.

<br>

## 2. 구현

draw 방식은 glDrawElements로<br>
삼각형 2개를 indexed drawing 하였습니다.<br>
즉, 버텍스를 4개만 사용하였습니다.


<br>그라데이션 효과자체는 메인 렌더루프에서 구현하지 않았습니다.

물론 루프에서<br>
시간(glfwGetTime)과 cmath의 cos, sin을 적절히 섞어<br>
유니폼으로 던져줄 수 있었으나<br>
셰이더에서 처리하는 게 코드상 깔끔한 것 같아서 였습니다.

<br>vertex attribute에 컬러 데이터를 넣어주지 않고 오직 position값만 던져주었습니다.

// main.cpp
<!-- snippet/resources/gradation/1.png -->


컬러처리는 vertex shader에서
layout으로 받아와 (ourPos)
단순히 fragment shader로 포워딩해줬습니다.

<br>

// main.fs
<!-- snippet/resources/gradation/2.png -->

입문자인 저로서는 vec3(0,2,4)라는 어떻게 저리 딱맞는 보정값을 찾아준 건지 신기하기만하네요.<br>
혹시 틀리거나 부족하게 설명된 부분이있다면 가감없이 지적해주시면 감사하겠습니다.
