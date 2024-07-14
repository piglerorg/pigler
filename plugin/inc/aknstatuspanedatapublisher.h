/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  AVKON internal publisher for system owned status pane data.
*
*/


#ifndef C_AKNSTATUSPANEDATAPUBLISHER_H
#define C_AKNSTATUSPANEDATAPUBLISHER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32property.h>
#include <AknPreviewPopUpObserver.h>

class CAknStatusPanePublisherData;
class CAknIndicatorPlugin;
class CAknIndicatorPopup;

/**
* This class is used to publish system controlled status pane data.
*
* @since S60 3.2
* @lib avkon.lib
*
*/
NONSHARABLE_CLASS( CAknStatusPaneDataPublisher ) : public CBase,
                                                   public MAknPreviewPopUpObserver
    {
public:

    /**
    * Two-phased constructor.
    */
    IMPORT_C static CAknStatusPaneDataPublisher* NewL();

    /**
    * Destructor.
    */
    ~CAknStatusPaneDataPublisher();

    /**
    * Publishes the current data.
    */
    IMPORT_C void PublishDataL();

    /**
    * Sets the ID of the current foreground app's subscriber.
    *
    * @param  aId  ID of the subscriber.
    */
    IMPORT_C void SetForegroundSubscriberId( TInt aId );

    /**
    * Sets the battery level.
    *
    * @param  aLevel  New battery level.
    */
    IMPORT_C void SetBatteryLevel( TInt aLevel );

    /**
    * Sets the battery re-charge indication on/off.
    *
    * @param  aRecharging  @c ETrue to set recharging animation on,
    *                      @c EFalse otherwise.
    */
    IMPORT_C void SetRecharge( TBool aRecharging );

    /**
    * Sets the signal level.
    *
    * @param  aLevel  New signal level.
    */
    IMPORT_C void SetSignalLevel( TInt aLevel );

    /**
    * Sets the signal icon (GPRS, EDGE, WCDMA, etc.)
    *
    * @param  aIcon  The icon state to be set.
    */
    IMPORT_C void SetSignalIcon( TInt aIcon );

    /**
    * Sets flags for the status pane incall bubble.
    *
    * @param  aFlags  Flags to be set, @see TAvkonIncallStatusBubbleFlags
    *                 in @c avkon.hrh.
    */
    IMPORT_C void SetIncallBubbleFlags( TInt aFlags );

    /**
    * Sets wheter incall status bubble is to be shown/not shown
    * in "usual" status pane layouts.
    *
    * @param  aAllowed  @c ETrue to shown incall status bubble in
    *                   "usual" status pane layouts, @c EFalse
    *                   otherwise.
    */
    IMPORT_C void SetIncallBubbleAllowedInUsual( TBool aAllowed );

    /**
    * Sets wheter incall status bubble is to be shown/not shown
    * in "usual" status pane layouts.
    *
    * @param  aAllowed  @c ETrue to shown incall status bubble in
    *                   "usual" status pane layouts, @c EFalse
    *                   otherwise.
    */
    IMPORT_C void SetIncallBubbleAllowedInUsual( TBool aAllowed, const TUint64& aClientId );
    
    /**
    * Sets wheter incall bubble is to be shown/not shown
    * in "idle" status pane layouts.
    *
    * @param  aAllowed  @c ETrue to shown incall status bubble in
    *                   "idle" status pane layouts, @c EFalse
    *                   otherwise.
    */
    IMPORT_C void SetIncallBubbleAllowedInIdle( TBool aAllowed );

    /**
    * Sets status indicator state.
    * Possible states are:
    *   - EAknIndicatorStateOff
    *   - EAknIndicatorStateOn
    *   - EAknIndicatorStateAnimate
    *
    * @param  aUid    UID of the indicator.
    * @param  aState  New state of the indicator, @see indicator states
    *                 in @c avkon.hrh.
    */
    IMPORT_C void SetIndicatorState( TUid aUid, TInt aState );

    /**
    * Handles indicator tap event.
    *
    * @param  aIndicatorUid  UID of the indicator which received the tap event.
    */
    IMPORT_C void HandleIndicatorTapL( TUid aIndicatorUid );

    /**
    * Clears status indicator states.
    */
    void ClearIndicatorStates();

    /**
    * Sets the battery icon state.
    * Possible states are:
    *   - EAknBatteryIndicatorNormal
    *   - EAknBatteryIndicatorPowerSave
    *
    * @param  aIcon  ID of the battery state, @see TAknBatteryIndicatorStates
    *                in @c avkon.hrh.
    */
    IMPORT_C void SetBatteryIcon( TInt aIcon );
    
    /**
    * Calculate and set the indicator popup's position 
    * 
    */
    IMPORT_C void SetIndicatorPopupPosition();
    
public: // From base class @c MAknPreviewPopUpObserver.

    /**
    * Called by the preview popup when an appropriate event takes place.
    *
    * @param  aController  Pointer to the sender of the event.
    * @param  aEvent       Event type.
    */
    virtual void HandlePreviewPopUpEventL(
        CAknPreviewPopUpController* aController,
        TPreviewPopUpEvent aEvent );

private:

    /**
    * Data structure to hold all indicators because only visible ones
    * (currently 3-6 depending on the layout) in the UI are published.
    */
    struct TAknStatusIndicator
        {
        TAknStatusIndicator() : iUid( 0 ),
                                iNarrowPriority( 0xFF ),
                                iWidePriority( 0xFF ),
                                iState( 0 ),
                                iPluginLoaded( EFalse ),
                                iPlugin( NULL ),
                                iPopupItemExists( EFalse )
        {};

        TInt                 iUid;
        TInt                 iNarrowPriority;
        TInt                 iWidePriority;
        TInt                 iState;
        TBool                iPluginLoaded;
        CAknIndicatorPlugin* iPlugin; // Own
        TBool                iPopupItemExists;
        };

    /**
    * Symbian second-phase constructor.
    */
    void ConstructL();

    /**
    * Standard constructor
    */
    CAknStatusPaneDataPublisher();

    /**
    * Sets status indicator state.
    * Possible states are:
    *   - EAknIndicatorStateOff
    *   - EAknIndicatorStateOn
    *   - EAknIndicatorStateAnimate
    *
    * @param  aUid    UID of the indicator.
    * @param  aState  New state of the indicator, @see indicator states
    *                 in @c avkon.hrh.
    */
    void SetIndicatorStateL( TUid aUid, TInt aState );

    /**
    * Prioritizes status indicators.
    */
    void PrioritizeIndicatorsL();

    /**
    * Loads the ECOM-plugin of an indicator.
    *
    * @param  aIndicator  @c TAknStatusIndicator data structure
    *                     of the indicator.
    */
    void LoadIndicatorPlugin( TAknStatusIndicator& aIndicator );
    
    /**
    * Gets the default description for a status indicator
    * to be displayed in the status indicator popup.
    *
    * @param          aUid   UID of the indicator.
    * @param[in,out]  aText  Text to be displayed in the popup.
    */
    void GetDefaultIndicatorTextL( TInt aUid, TDes& aText );

private:

    /** Property for server updated status pane state data. */
    RProperty iProperty;

    /** Data structure to hold status pane state data. */
    CAknStatusPanePublisherData* iStatusPaneStateData;

    /** Array containing the status indicators information. */
    RArray<TAknStatusIndicator> iIndicators;
    
    /**
    * Universal status indicator popup.
    * Own.
    */
    CAknIndicatorPopup* iPopup;

    /**
    * Handle to the Autolock state property.
    * This is checked to disable universal indicator popup
    * when device lock is active.
    */ 
    RProperty iAutolockStateProperty;
    };

#endif // C_AKNSTATUSPANEDATAPUBLISHER_H