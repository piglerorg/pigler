#include <PiglerSession.h>

int main() 
{
	PiglerRequest request;
    PiglerSession *session = new PiglerSession;
    session->Connect();
    session->SendStruct(request);
}