/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#ifndef __AKNNOTIFY_H__
#define __AKNNOTIFY_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS 
class CAknSDData;

// CLASS DECLARATION
class RNotifier2 : public RNotifier
    {
public:
    TInt Connect();
    };
    
/**
*  Base class for handling @c RNotifier.
*
*  @since S60 0.9
*/
NONSHARABLE_CLASS(CAknNotifyBase) : public CBase
    {
public:
    /**
    * Destructor.
    */
    IMPORT_C ~CAknNotifyBase();

    /**
     * @since S60 3.1
     * Sets additional information to be sent to secondary display.
     * Takes ownership of object.
     * Must be called before sending data to notifier to have effect.
     * @internal to S60
     *
     * @aparam aData data to be sent to cover UI
     */
    IMPORT_C void SetSecondaryDisplayData(CAknSDData* aData);

protected:
    /**
    * C++ constructor.
    * @param aUid The UID identifying the plug-in notifier.
    */
    CAknNotifyBase(TUid aUid);
    
    /**
    * 2nd phase constructor.
    * Connects to the notifier server.
    */
    void ConstructL();
    
    /**
    * Starts or updates the notifier.
    * @param aBuffer Data that can be passed to notifier.
    * @param aResponse Data that can be returned by notifier.
    */
    void StartOrUpdateL(const TDesC8& aBuffer, TDes8& aResponse);
    
    /**
    * Starts the notifier.
    * @param aStatus The request status.
    * @param aBuffer Data that can be passed to the notifier.
    * @param aResponse Data that can be returned by the notifier.
    */
    void StartOrUpdateAndGetResponseL(
        TRequestStatus& aStatus,
        const TDesC8& aBuffer,
        TDes8& aResponse);
                                                                            
    // We don't give this funtionality out of library at the moment
    // returns instance of secondary display data if set.
    CAknSDData* SecondaryDisplayData();
                                                                          
private:
    IMPORT_C void CAknNotifyBase_Reserved();

private:
    TUid iUid;
    TBool iStarted;
    RNotifier2 iNotify;
    CAknSDData* iSecondaryDisplayData;
    };

#endif // __AKNNOTIFY_H__
