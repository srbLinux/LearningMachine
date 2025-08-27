#ifndef LMDEF_H
#define LMDEF_H

#ifdef LM_DEBUG
 #ifdef LM_AUTHOR_SRB
  #define HTTPS_REQ_HEAD ""
 #elif LM_AUTHOR_GX
 #endif
#else
 #define HTTPS_REQ_HEAD "api.learningmachine.com"
#endif

#endif // LMDEF_H
