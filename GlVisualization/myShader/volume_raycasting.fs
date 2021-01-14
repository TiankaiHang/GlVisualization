// ref: https://github.com/toolchainX/Volume_Rendering_Using_GLSL/blob/master/shader/raycasting.frag
// #version 400
#version 330 core
// 杜绝声明未使用的变量，避免bug的产生。


in vec3 EntryPoint;
in vec4 ExitPointCoord;

uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;  
uniform float     StepSize = 0.001f;
uniform vec2      ScreenSize;
layout (location = 0) out vec4 FragColor;

void main()
{
    // ExitPointCoord 的坐标是设备规范化坐标
    // 出现了和纹理坐标有关的问题。
    vec3 exitPoint = texture(ExitPoints, gl_FragCoord.xy/ScreenSize).xyz;
    // that will actually give you clip-space coordinates rather than
    // normalised device coordinates, since you're not performing the perspective
    // division which happens during the rasterisation process (between the vertex
    // shader and fragment shader
    // vec2 exitFragCoord = (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0;
    // vec3 exitPoint  = texture(ExitPoints, exitFragCoord).xyz;
    if (EntryPoint == exitPoint)
    	//background need no raycasting
    	discard;
    vec3 dir = exitPoint - EntryPoint;
    float len = length(dir); // the length from front to back is calculated and used to terminate the ray
    vec3 deltaDir = normalize(dir) * StepSize;
    float deltaDirLen = length(deltaDir);
    vec3 voxelCoord = EntryPoint;
    vec4 colorAcum = vec4(0.0); // The dest color
    float alphaAcum = 0.0;                // The  dest alpha for blending
    // 定义颜色查找的坐标
    float intensity;
    float lengthAcum = 0.0;
    vec4 colorSample; // The src color 
    float alphaSample; // The src alpha
    // backgroundColor
    vec4 bgColor = vec4(1.0, 1.0, 1.0, 0.0);
 
    // FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    for(int i = 0; i < 1600; i++)
    {
    	// 获得体数据中的标量值scaler value
    	intensity =  texture(VolumeTex, voxelCoord).x;
        //if(intensity < -0.0f){
        //    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);}
    	// 查找传输函数中映射后的值
    	// 依赖性纹理读取  
    	colorSample = texture(TransferFunc, intensity) * 256.0f / 200.0f;
    	// modulate the value of colorSample.a
    	// front-to-back integration
    	if (colorSample.a > 0.0) {
    	    // accomodate for variable sampling rates (base interval defined by mod_compositing.frag)
    	    colorSample.a = 1.0 - pow(1.0 - colorSample.a, StepSize*200.0f);
    	    colorAcum.rgb += (1.0 - colorAcum.a) * colorSample.rgb * colorSample.a;
    	    colorAcum.a += (1.0 - colorAcum.a) * colorSample.a;
    	}
    	voxelCoord += deltaDir;
    	lengthAcum += deltaDirLen;
    	if (lengthAcum >= len )
    	{	
    	    colorAcum.rgb = colorAcum.rgb*colorAcum.a + (1 - colorAcum.a)*bgColor.rgb;		
    	    break;  // terminate if opacity > 1 or the ray is outside the volume	
    	}	
    	else if (colorAcum.a > 1.0)
    	{
    	    colorAcum.a = 1.0;
    	    break;
    	}
    }
    FragColor = colorAcum;
    // for test
    // FragColor = vec4(EntryPoint, 1.0); // have color
    // FragColor = vec4(vec3(colorSample), 1.0);
    // FragColor = vec4(ExitPointCoord); // have  color and color changes
    // FragColor = vec4(exitPoint, 1.0f); //for debug, have color now!~
    // FragColor = vec4(gl_FragCoord.xy/ScreenSize, 1.0f, 1.0f);
    // FragColor = vec4(-dir, 1.0f);
    // FragColor = vec4(intensity, intensity, intensity, intensity);
};