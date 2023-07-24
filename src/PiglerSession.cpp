#include "PiglerSession.h"
#include "PiglerPlugin.h"

PiglerSession::PiglerSession(PiglerPlugin* plugin) : CSession2(), plugin(plugin)
{
}

void PiglerSession::ServiceL(const RMessage2& message)
{
    switch(message.Function()) {
    case EPostItem: {
        PiglerRequest request;
        TPckg<PiglerRequest> data(request);
        message.ReadL(0, data);
        plugin->iText = request.text;
        TInt code;
        TRAP(code, code = plugin->AddStatusPanelItemL());
        message.Complete(code);
        break;
    }
    case ERemoveItem: {
        PiglerRequest request;
        TPckg<PiglerRequest> data(request);
        message.ReadL(0, data);
        plugin->RemoveStatusPanelItem(request.uid);
        message.Complete(0);
        break;
    }
    case EUpdate: {
        PiglerRequest request;
        TPckg<PiglerRequest> data(request);
        message.ReadL(0, data);
        TInt code;
        TRAP(code, plugin->UpdateL(request.uid));
        message.Complete(code);
        break;
    }
    }
    message.Complete(-1);
}

void PiglerSession::ServiceError(const RMessage2& message, TInt error)
{
	message.Complete(error);
}
