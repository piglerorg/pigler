#ifndef PIGLERSESSION_H
#define PIGLERSESSION_H

#include <e32base.h>

class PiglerRequest
    {
public:
    TInt op;
    TInt id;
    TPtrC name;
    TPtrC icon;
    };

class PiglerSession : public RSessionBase
{
public:
    TInt Connect();
    TInt SendStruct(const PiglerRequest request);
};

#endif // PIGLERSESSION_H
