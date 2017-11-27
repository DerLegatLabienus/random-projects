#include <gcj/cni.h>
     #include <java/lang/System.h>
     #include <java/util/Stack.h>
     #include <java/io/PrintStream.h>
     #include <java/lang/Throwable.h>
     #include "Person.h"
     
     int main(int argc, char *argv[])
     {
       using namespace java::lang;
       using namespace java::util;
     
       try
       {
         JvCreateJavaVM(NULL);
         JvAttachCurrentThread(NULL, NULL);
     
         String *message = JvNewStringLatin1("Hello from C++");
         JvInitClass(&System::class$);
         JvInitClass(&Stack::class$);
         System::out->println(message);

	 Person *p = new Person();
	 Stack *stack = new Stack();
	 System::out->println(p->namewhatsoever);
	 stack->push(p);
	 System::out->println(stack->pop());
     
         JvDetachCurrentThread();
       }
       catch (Throwable *t)
       {
         System::err->println(JvNewStringLatin1("Unhandled Java exception:"));
         t->printStackTrace();
       }
     }
