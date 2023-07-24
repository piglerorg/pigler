#include "PiglerSession.h"
#include "PiglerPlugin.h"

PiglerSession::PiglerSession(PiglerPlugin* plugin) : CSession2(), plugin(plugin)
{
}

void PiglerSession::ServiceL(const RMessage2& message)
{
    switch(message.Function()) {
    case EPostItem: {
        TPiglerNotification request;
        TPckg<TPiglerNotification> data(request);
        message.ReadL(0, data);
        plugin->NewItem(request);
        TInt code;
        TRAP(code, request.uid = code = plugin->AddStatusPanelItemL());
        // отправить обратно
        TPckg<TPiglerNotification> data2(request);
        message.WriteL(0, data2);
        message.Complete(code);
    }
    break;
    case ERemoveItem: {
        TPiglerNotification request;
        TPckg<TPiglerNotification> data(request);
        message.ReadL(0, data);
        plugin->RemoveItem(request);
        plugin->RemoveStatusPanelItem(request.uid);
        message.Complete(0);
    }
    break;
    case EUpdateItem: {
        TPiglerNotification request;
        TPckg<TPiglerNotification> data(request);
        message.ReadL(0, data);
        plugin->UpdateItem(request);
        TInt code;
        TRAP(code, plugin->UpdateL(request.uid));
        message.Complete(code);
    }
    break;
    default: { // типо еррор
        message.Complete(-1);
    }
    }
    /*
    TPiglerRequest request;
    TPckg<TPiglerRequest> data(request);
    message.ReadL(0, data);
    plugin->iText = request.text;
    TInt code;
    TRAP(code, code = plugin->AddStatusPanelItemL());
    message.Complete(code);
    */
}

void PiglerSession::ServiceError(const RMessage2& message, TInt error)
{
	message.Complete(error);
}
