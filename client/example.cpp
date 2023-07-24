#include <PiglerClient.h>

int main() 
{
    PiglerClient *client = new PiglerClient;
    client->Connect();
    _LIT(KTest, "Some some\nasd asd dsa")
    client->Post(KTest().AllocL());
}
