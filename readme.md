## OpenGL学习记录工程

记录下目前OpenGL的学习进度，克服拖延症:)



### 目前已经实现的功能：

1. 着色器类
2. 纹理类
3. 摄像机
4. 基础光照（Ambient&Diffuse&Specular）&材质
5. 光照贴图
6. 多光源，点光源，聚光灯
7. 模型类
8. 深度测试&模板测试&混合
9. 帧缓冲
10. Gamma矫正



**Todo:**

1. GPU Instance
2. 延迟渲染
3. PBR
4. 光线追踪
5. ...



### 效果图：

![image-20220323221256224](./outPic/image-20220323221256224.png)



反相:

![image-20220323221852457](./outPic/image-20220323221852457.png)



Kernel-Blur

![image-20220323221953333](./outPic/image-20220323221953333.png)



Gamma矫正：

![image-20220323222246265](./outPic/image-20220323222246265.png)



### 项目结构（平台为x86）

1. Dependencies

2. includes

   功能类以及三方库(assimp&stbimage)的头文件

3. resources

   美术资源，贴图、模型等

4. main.cpp

   GLEW&GLAD初始化工作，渲染的生命周期维护

5. Scene.h

   场景参数、必要的头文件引入、核心方法声明

6. Scene.cpp

   绘制核心方法实现