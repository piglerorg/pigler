/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/

#ifndef C_AKNSMALLINDICATOR_H
#define C_AKNSMALLINDICATOR_H

#include "AknNotify.h"

NONSHARABLE_CLASS(CAknSmallIndicator) : public CAknNotifyBase
    {
public:
    
    /**
     * Two-phased constructor.
     *
     * @param  aIndicatorUid  UID of the status indicator.
     * @return Pointer to a new @c CAknSmallIndicator instance.
     */
    IMPORT_C static CAknSmallIndicator* NewL( TUid aIndicatorUid );
    
    /**
     * Two-phased constructor. Leaves the created instance
     * on the cleanup stack.
     *
     * @param  aIndicatorUid  UID of the status indicator.
     * @return Pointer to a new @c CAknSmallIndicator instance.
     */
    IMPORT_C static CAknSmallIndicator* NewLC( TUid aIndicatorUid );
    
    /**
     * Destructor.
     */
    IMPORT_C ~CAknSmallIndicator();
    
    /**
     * Sets the state of the status indicator.
     *
     * @param  aState  The state to be set to the indicator.
     *                 @see MAknIndicator::TAknIndicatorState
     */
    IMPORT_C void SetIndicatorStateL( const TInt aState );
    
    /**
     * Handles stylus tap event on the indicator.
     */
    IMPORT_C void HandleIndicatorTapL();
    
private:
    CAknSmallIndicator();
    void ConstructL( TUid aIndicatorUid );
    
private:
    IMPORT_C void CAknNotifyBase_Reserved();

private:
    TUid iIndicatorUid;
    };

#endif // C_AKNSMALLINDICATOR_H
