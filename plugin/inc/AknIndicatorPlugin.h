/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This software, including documentation is protected by copyright controlled
* by Nokia Corporation. Copying, including reproducing, storing, adapting or
* translating, any or all of this material can only be done in accordance
* with the Nokia Symbian License version 1.0 (or any subsequent update) or
* any other license terms expressly agreed between you and Nokia.
* This material contains Nokia's confidential information which
* may not be disclosed to others without the prior written consent of Nokia.
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Indicator ECOM Plugin interface declaration.
*
*/


#ifndef C_AKNINDICATORPLUGIN_H
#define C_AKNINDICATORPLUGIN_H

// INCLUDES
#include <ecom/ecom.h>
#include <AknIndicatorPluginImplUIDs.hrh>

// FORWARD DECLARATIONS
class CGulIcon;
class CAknIndicatorPopupContent;

// CONSTANTS
// Interface UID for Indicator Plugin interface
const TUid KAknIndicatorPluginIfUid = { KAknIndicatorPluginInterfaceUid };

/**
*  @c CAknIndicatorPlugin has to be implemented
*  by each ECOM based indicator tap handler plugin.
*
*  A single indicator ECOM plugin can implement the functionality
*  for multiple indicators, since the indicator UID is always passed to
*  the interface.
*
*  Each indicator ECOM plugin has to implement the @c HandleIndicatorTapL()
*  method, as well as the TextL() method for at least indicators
*  which have a function link
*
*  The implementation for @c HandleIndicatorTapL() should provide eg. a
*  direct access to the indicator-related settings or
*  launch the application whose indicator is in question.
*
*  When an installable update of indicator plugin is provided, it is
*  recommended that implementation version in plugin resource file should
*  be increased. This way updated plugin will be loaded by the platform 
*  as soon as it installed.
*
*  @lib avkon.lib
*  @since 5.0
*/
class CAknIndicatorPlugin : public CBase
{
public:

    enum TAknIndicatorPluginTextTypes
        {
        /** Link text items are shown on the status panel */
        EAknIndicatorPluginLinkText = 1,
        /** Information item without link. These items are not shown on status panel */
        EAknIndicatorPluginInformationText
        };

    enum TAknIndicatorStatusPanelItems
        {
        /** Value for invalid indicator uid. */
        EAknIndicatorStatusPanelItemInvalid = -1        
        };
    
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    *
    * @param  aImplUid  Implementation UID of the plugin.
    *
    * @return  Pointer to an object of @c CAknIndicatorPlugin.
    */
    IMPORT_C static CAknIndicatorPlugin* NewL( const TUid& aImplUid );

    /**
    * Destructor.
    */
    IMPORT_C virtual ~CAknIndicatorPlugin();

public: // New functions

    /**
    * Implements tap actions for the indicator in question (@c aUid).
    *
    * @param  aUid  UID of the indicator.
    */
    virtual void HandleIndicatorTapL( const TInt aUid ) = 0;
    
    /**
    * Returns the description used as link text, displayed with
    * the indicator icon in the indicator popup.
    *
    * @param       aUid       UID of the indicator.
    * @param[out]  aTextType  Type of the text,
    *                         @see TAknIndicatorPluginTextTypes.
    *
    * @return  Indicator description or link text.
    *          Ownership is transferred.
    */
    IMPORT_C virtual HBufC* TextL( const TInt aUid, TInt& aTextType );
    
    /**
    * Returns the indicator icon used in the indicator popup.
    * If the implementation return @c NULL, then the
    * indicator pane icon is used in the popup.
    *
    * The implementation should always re-create the icon
    * when this method is called, because the color of the
    * icon is updated by calling this method when skin changes.
    *
    * @param   aUid  UID of the indicator.
    *
    * @return  The indicator icon.
    *          Ownership is not transferred.
    */
    IMPORT_C virtual const CGulIcon* IconL( const TInt aUid );
    
    /**
     * Returns indicator value for the indicator popup.
     * 
     * @param   aUid  UID of the indicator.
     * 
     * @return  Indicator value.
     */
    IMPORT_C TInt ValueL( const TInt aUid );
    
    /**
    * Method to update the plugin's data in the indicator popup.
    *
    * When an indicator's icon or text changes, the plugin
    * implementation for that indicator should call this method.
    *
    * @param  aUid  UID of the indicator to be updated.
    */
    IMPORT_C void UpdateL( TInt aUid );
    
    /**
     * Adds an indicator item to indicator popup (status panel) list. 
     * This API is used when there isn't indicator in universal indicator pane.
     * 
     * The method cannot be called from indicator plugin constructor.
     * If there is need for adding the list indicator at the boot time,
     * you need to use extension mechanism with extension id 
     * KAknIndicatorPluginExtensionIdPostConstruct to call the API.
     * 
     * You need to implement also TextL() with text type EAknIndicatorPluginLinkText
     * and IconL() methods to show the item in the list. You may update the list
     * with UpdateL() call.
     *  
     * @return  Dynamic uid for the indicator item.
     */
    IMPORT_C TInt AddStatusPanelItemL();

    /**
     * Removes an indicator item from status panel list.
     * You need to use the same uid that was returned
     * from AddStatusPanelItemL() method.
     * 
     * @param  aUid Uid of the indicator to be removed.
     */
    IMPORT_C void RemoveStatusPanelItem( TInt aUid );
    
    /**
    * Set the observer of the plugin's data.
    *
    * @internal This is automatically called by the framework when loading
    *           the plugin, and should not be overridden by the plugin.
    *
    * @param  aPluginObserver  Pointer to the observer.
    */
    void SetPluginObserver( CAknIndicatorPopupContent* aPluginObserver );

    /**
     * The method is called after the plugin has been created. 
     */
    void PostConstructL();

private: // Member data

    /** Implementation UID of the indicator tap handler. */
    TUid iDtor_ID_Key;
    
    /**
    * Pointer to the plugin's indicator data change observer.
    * Not own.
    */
    CAknIndicatorPopupContent* iPluginObserver;

};

#endif  // C_AKNINDICATORPLUGIN_H

// End of File
