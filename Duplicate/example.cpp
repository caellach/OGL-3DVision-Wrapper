// INFO regarding 
https://www.khronos.org/opengles/sdk/docs/man3/html/glGetFramebufferAttachmentParameteriv.xhtml

void init_rendered_FBO() 
{
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1,&fbo_tex);
    glBindTexture(GL_TEXTURE_2D, fbo_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_screen, height_screen, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width_screen, height_screen);
    glBindTexture (GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex, 0);
    
    
    int objectType;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,&objectType);
    ASSERT(objectType == GL_TEXTURE);
    
    
    int objectName;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&objectName);
    ASSERT(glIsTexture(objectName) == GL_TRUE);
    int wid, hei, fmt;
    
    glBindTexture(GL_TEXTURE_2D, objectName);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &wid);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &hei);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT,
            &fmt);
    glBindTexture(GL_TEXTURE_2D, 0);
    std::string format = convertInternalFormatToString(fmt);
    std::cout << "Color attachment 0: " << objectName << " " << wid << "x" << hei << ", " << format << std::endl;
    ASSERT(checkFramebufferStatus());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// this is the init function that gets called.
void init_rendered_multisample_FBO() {
    init_rendered_FBO();
    // now I'm going to set up the additional component necessary to perform multisampling which is a new fbo
    // that has a multisampled color buffer attached. I won't need a multisample depth buffer.
    glGenFramebuffers(1,&multi_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,multi_fbo);
    glGenRenderbuffers(1,&renderbuffer_multi);
    glBindRenderbuffer(GL_RENDERBUFFER,renderbuffer_multi);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA, width_screen,height_screen);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer_multi);
    
    // Getting the state
    int objectType;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,&objectType);
    ASSERT(objectType == GL_RENDERBUFFER);
    
    int objectName;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&objectName);
    ASSERT(glIsRenderbuffer(objectName) == GL_TRUE);
    
    glBindRenderbuffer(GL_RENDERBUFFER,objectName);
    int wid,hei,fmt,sam;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_WIDTH,&wid);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_HEIGHT,&hei);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_INTERNAL_FORMAT,&fmt);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER,GL_RENDERBUFFER_SAMPLES,&sam);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    printf("Renderbuffer: %dx%d, fmt=%d, samples=%d\n",wid,hei,fmt,sam);
    ASSERT(checkFramebufferStatus());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
}
// this is called after rendering to multi_fbo
void resolve_multisample_FBO() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, multi_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    GLenum drawbuf = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1,&drawbuf);
    glBlitFramebuffer(0,0,width_screen,height_screen,0,0,width_screen,height_screen,GL_COLOR_BUFFER_BIT,GL_NEAREST);
}