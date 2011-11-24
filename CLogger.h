/* logging
*/

#ifndef __CLogger_h_
#define __CLogger_h_

#include <windows.h>
#include <fstream>

class CLogger
{
 //Handle MyFile;
 
 public:
        void LogThis(char * text);
        void LogThisFloat(float f);
};
      

#endif
