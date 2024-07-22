/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Server side status pane.
*
*/


#ifndef C_EIKSRVSP_H
#define C_EIKSRVSP_H

#include <eikspane.h>
#include <AknSgcc.h>

class CAknStatusPaneDataPublisher;

class MEikServStatusPaneShapeChangeObserver
    {
public:
    virtual void HandleShapeChangeL() const = 0;
    };

/**
 * @c CEikServStatusPaneModel is the server side version of
 * the status pane model class.
 */
class CEikServStatusPaneModel : public CEikStatusPaneModelBase
    {
public:

    /**
    * Two-phased constructor.
    *
    * @param  aEikEnv         An environment for creating controls and utility
    *                         functions for manipulating them.
    * @param  aCoreResId      Status pane core resource ID.
    * @param  aShapeObserver  Status pane shape change observer.
    *
    * @return Pointer to the created @c CEikServStatusPaneModel object.
    */
    static CEikServStatusPaneModel* NewL(
        CEikonEnv& aEikEnv, 
        TInt aCoreResId, 
        const MEikServStatusPaneShapeChangeObserver& aShapeObserver );
    
    /**
    * Destructor.
    */
    ~CEikServStatusPaneModel();

    /**
    * Sets the status pane layout of the server side status pane model.
    *
    * @param  aLayoutResId          Resource ID of the layout to be set.
    * @param  aChangeStatusPaneNow  A flag to indicate an immediate status
    *                               pane layout change.
    */
    void SetLayoutL( TInt aLayoutResId, TBool aChangeStatusPaneNow );

private:

    /**
    * C++ constructor.
    *
    * @param  aEikEnv         An environment for creating controls and utility
    *                         functions for manipulating them.
    * @param  aShapeObserver  Status pane shape change observer.
    */
    CEikServStatusPaneModel(
        CEikonEnv& aEikEnv, 
        const MEikServStatusPaneShapeChangeObserver& aShapeObserver );

    
    /**
    * Second-phase constructor.
    *
    * @param  aCoreResId  Status pane core resource ID.
    */
    void ConstructL( TInt aCoreResId );

private: // Member data.

    /** Status pane shape change observer. */
    const MEikServStatusPaneShapeChangeObserver& iShapeObserver;
    };


/**
 * @c CEikServStatusPane is the server side version of the status pane.
 * This will typically used to own sub panes that contains
 * system wide information.
 */
class CEikServStatusPane : public CEikStatusPaneBase,
                           public MAknSgcStatusPaneRedrawCoordinator, 
                           public MEikServStatusPaneShapeChangeObserver
    {
public:

    /**
    * Two-phased constructor.
    *
    * @param  aEikEnv     An environment for creating controls and utility
    *                     functions for manipulating them.
    * @param  aParent     Pointer to the parent window group.
    * @param  aCoreResId  Status pane core resource ID.
    *
    * @return Pointer to the created @c CEikServStatusPane object.
    */
    static CEikServStatusPane* NewL( CEikonEnv& aEikEnv,
                                     RWindowGroup* aParent,
                                     TInt aCoreResId );
    
    /**
    * Destructor.
    */
    ~CEikServStatusPane();
    
public: // New functions.

    /**
    * Sets the status pane observer for server side status pane.
    * The observer is server side screen blanker, as there is no
    * @c CAknAppUi.
    *
    * @param  Pointer to the status pane observer.
    */
    void SetBlankWinObserver( MEikStatusPaneObserver* aBlankWinObserver );
    
    /**
    * Prepares the status pane for the application exit.
    */
    void PrepareForAppExit();
    
    /**
    * Sets the current status pane layout resource ID of the application
    * side status pane.
    *
    * @param  aResourceId  Resource ID to be set.
    */
    void SetCurrentAppStatuspaneResourceId( TInt aResourceId );
    
    /**
    * Returns the current status pane layout resource ID of the application
    * side status pane.
    *
    * @return Current application side status pane layout resource ID.
    */
    TInt CurrentAppStatuspaneResourceId();  
    
public: // From base class @c CEikStatusPaneBase.

    /**
    * Not implemented, as fading is handled by Window Server and
    * the Application Framework.
    *
    * @param  aFaded  Not used.
    */
    void SetFaded( TBool aFaded );
    
    /**
    * Handles resource change events.
    *
    * @param  aType  Event type.
    */
    void HandleResourceChange( TInt aType );
    
    void SwitchLayoutL( TInt aLayoutResourceId );
    
public: // From base class @c MAknSgcStatusPaneRedrawCoordinator.

    /**
    * Not implemented, server side status pane redraw blocker is not
    * used anymore.
    */
    void BlockServerStatusPaneRedrawsL();
    
    /**
    * Redraws the server side status pane.
    * Currently does nothing, as all status pane drawing happens
    * on the application side.
    */
    void RedrawServerStatusPane();

public: // From base class @c MEikServStatusPaneShapeChangeObserver.
    
    /**
    * Handles event caused by status pane shape change.
    */
    void HandleShapeChangeL() const;
    
private:

    /**
    * C++ constructor.
    *
    * @param  aEikEnv     An environment for creating controls and utility
    *                     functions for manipulating them.
    * @param  aParent     Pointer to the parent window group.
    */
    CEikServStatusPane( CEikonEnv& aEikEnv, RWindowGroup* aParent );
    
    /**
    * Second-phase constructor.
    *
    * @param  aCoreResId  Status pane core resource ID.
    */
    void ConstructL( TInt aCoreResId );
    
private: // From base class @c CEikStatusPaneBase.

    /**
    * Creates a new model for the server side status pane.
    *
    * @param  aCoreResId  ID for the status pane resource.
    */
    CEikStatusPaneModelBase* CreateModelL(TInt aCoreResId) const;
    
    
    /**
    * Can be used by the base class pointer to determine whether or
    * not the status pane is on application side or server side.
    *
    * @return @c ETrue.
    */
    TBool IsApp() const;
    
private: // Member data.
    
    /** Not used. */
    TBool iIsFaded;
    
    /** Status pane observer, screen blanker. */
    MEikStatusPaneObserver* iBlankWinObserver;
    
    /** Current application side status pane layout resource ID. */
    TInt iCurrentAppStatuspaneResourceId;
    
public: // Member data.
    
    /**
    * Status pane data publisher for system controlled status pane data.
    * Own.
    */
    CAknStatusPaneDataPublisher* iDataPublisher;
    };

#endif // C_EIKSRVSP_H
