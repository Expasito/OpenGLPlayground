# OpenGLPlayground

Basically, I want to figure the best drawing method for Render Project without messing that up and allowing for a smaller code best to test off of. Overall, the goals are to figure out a way to: <br>
(1) Allow for frustrum culling of 'chunks' where entities are put in chunks based on their location. <br>
(2) Find the best opengl drawing function for our situation. <br>
(3) Figure out the best data structure for hold entities, transforms, model matricies. <br>
(4) Anything else that I want to test for performance, such as moving model matricies over to CPU from GPU to avoid recomputing, using Indirect rendering vs Instancing, etc. <br>
<br>
<br>
This will probably be a very messy repo since it is just for testing whatever. I would not look at this repo for my typical coding style, along with the Render Project repo for a bit while I get that fixed. <br>
Hopefully, I will learn lots about how OpenGL draws things and about each drawing function and I probably will log my results in some files for future reference.


<br>
Here is a list of resources to remember for later: <br>
https://i.stack.imgur.com/JgrSc.jpg <br>
https://www.khronos.org/assets/uploads/developers/library/2013-siggraph-opengl-bof/Batch-and-Cull-in-OpenGL-BOF_SIGGRAPH-2013.pdf <br>
https://www.nvidia.com/docs/IO/8228/BatchBatchBatch.pdf <br>
https://on-demand.gputechconf.com/gtc/2014/presentations/S4379-opengl-44-scene-rendering-techniques.pdf <br>
https://www.slideshare.net/CassEveritt/approaching-zero-driver-overhead <br>
http://www.g-truc.net/post-0518.html <br>
https://on-demand.gputechconf.com/gtc/2013/presentations/S3032-Advanced-Scenegraph-Rendering-Pipeline.pdf <br>

