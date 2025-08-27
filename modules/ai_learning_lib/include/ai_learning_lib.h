#ifndef AI_LEARNING_LIB_H
#define AI_LEARNING_LIB_H

#include <QtCore>

namespace LM
{

#ifdef BUILD_AI_LEARNING_DLL
 #define AI_LEARNING_API Q_DECL_EXPORT
#else
 #define AI_LEARNING_API Q_DECL_IMPORT
#endif

class AI_LEARNING_API AILearning : public QObject
{
public:
    explicit AILearning(QObject *parent=NULL);

};

}

#endif
