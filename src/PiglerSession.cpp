#include "PiglerSession.h"
#include "PiglerPlugin.h"

PiglerSession::PiglerSession(PiglerPlugin* plugin) : CSession2(), plugin(plugin)
{
}

void PiglerSession::ServiceL(const RMessage2& message)
{
    switch(message.Function()) {
    case 1: {
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
    case 2: {
        TPiglerNotification request;
        TPckg<TPiglerNotification> data(request);
        message.ReadL(0, data);
        plugin->RemoveItem(request);
        plugin->RemoveStatusPanelItem(request.uid);
        message.Complete(0);
    }
    break;
    case 3: {
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
	TPiglerNotification request;
	TPckg<TPiglerNotification> data(request);
	message.ReadL(0, data);
	//plugin->NewItem(request);
    TInt code;
    TRAP(code, code = plugin->AddStatusPanelItemL());
    message.Complete(code);
    */
}

void PiglerSession::ServiceError(const RMessage2& message, TInt error)
{
	message.Complete(error);
}
