#version 450
#extension GL_ARB_separate_shader_objects:enable

layout(location=0)in vec3 inPosition;
// layout(location=1)in vec3 inColor;

layout(location=2)in vec2 inUV;

layout(location=0)out vec3 outColor;
layout(location=1)out vec2 outUV;

layout(binding=0)uniform VPMatrices{
    
    mat4 m_ViewMatrix;
    mat4 m_ProjectionMatrix;
}vpUBO;

layout(binding=1)uniform ObjectUniform{
    mat4 mModelMatrix;
}objectUBO;

void main(){
    
    gl_Position=vpUBO.m_ProjectionMatrix*vpUBO.m_ViewMatrix*objectUBO.mModelMatrix*vec4(inPosition,1.);
    // outColor = inColor;
    outUV=inUV;
    
}