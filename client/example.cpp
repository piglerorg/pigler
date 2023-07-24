#include <PiglerClient.h>

int main() 
{
    PiglerClient *client = new PiglerClient;
    client->Connect();
    _LIT(KTest, "Some some\nasd asd dsa")
    TPiglerNotification notification;
    notification.text = KTest();
    client->Post(notification);
}
