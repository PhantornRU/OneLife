#include "EditorObjectPage.h"

#include "minorGems/game/game.h"
#include "minorGems/game/Font.h"
#include "minorGems/game/drawUtils.h"

#include "minorGems/util/stringUtils.h"
#include "minorGems/util/SettingsManager.h"

#include "minorGems/graphics/converters/PNGImageConverter.h"

#include "minorGems/io/file/FileInputStream.h"



extern Font *mainFont;



EditorObjectPage::EditorObjectPage()
        : mDescriptionField( mainFont, 
                             0,  -260, 6,
                             false,
                             "Description", NULL, " " ),
          mSaveObjectButton( mainFont, 210, -260, "Save" ),
          mImportEditorButton( mainFont, 210, 260, "Sprites" ),
          mSpritePicker( -310, 100 ) {

    addComponent( &mDescriptionField );
    addComponent( &mSaveObjectButton );
    addComponent( &mImportEditorButton );
    addComponent( &mSpritePicker );

    mSaveObjectButton.addActionListener( this );
    mImportEditorButton.addActionListener( this );

    mSpritePicker.addActionListener( this );

    mSaveObjectButton.setVisible( false );


    mCurrentObject.id = -1;
    mCurrentObject.description = mDescriptionField.getText();
    mCurrentObject.numSprites = 0;
    mCurrentObject.sprites = new int[ 0 ];
    mCurrentObject.spritePos = new doublePair[ 0 ];

    mPickedObjectLayer = -1;
    }



EditorObjectPage::~EditorObjectPage() {

    delete [] mCurrentObject.description;
    delete [] mCurrentObject.sprites;
    delete [] mCurrentObject.spritePos;
    }








void EditorObjectPage::actionPerformed( GUIComponent *inTarget ) {
    
    if( inTarget == &mSaveObjectButton ) {
        }
    else if( inTarget == &mImportEditorButton ) {
        setSignal( "importEditor" );
        }
    else if( inTarget == &mSpritePicker ) {
        
        int spriteID = mSpritePicker.getSelectedSprite();
        
        if( spriteID != -1 ) {
            

            int newNumSprites = mCurrentObject.numSprites + 1;
            
            int *newSprites = new int[ newNumSprites ];
            memcpy( newSprites, mCurrentObject.sprites, 
                    mCurrentObject.numSprites * sizeof( int ) );
            
            doublePair *newSpritePos = new doublePair[ newNumSprites ];
            memcpy( newSpritePos, mCurrentObject.spritePos, 
                    mCurrentObject.numSprites * sizeof( doublePair ) );
        
            newSprites[ mCurrentObject.numSprites ] = spriteID;
            
            doublePair pos = {0,0};
            
            newSpritePos[ mCurrentObject.numSprites] = pos;

            delete [] mCurrentObject.sprites;
            delete [] mCurrentObject.spritePos;
            
            mCurrentObject.sprites = newSprites;
            mCurrentObject.spritePos = newSpritePos;
            mCurrentObject.numSprites = newNumSprites;
            
            mSpritePicker.unselectSprite();            
            }
        
        }
    
    
    }

   

     
void EditorObjectPage::draw( doublePair inViewCenter, 
                     double inViewSize ) {
    
    setDrawColor( 1, 1, 1, 1 );

    doublePair pos = { 0, 0 };
    
    drawObject( &mCurrentObject, pos );
    }



void EditorObjectPage::step() {
    }




void EditorObjectPage::makeActive( char inFresh ) {
    if( !inFresh ) {
        return;
        }


    }


void EditorObjectPage::pointerMove( float inX, float inY ) {
    }


void EditorObjectPage::pointerDown( float inX, float inY ) {

    doublePair pos = { inX, inY };
    
    mPickedObjectLayer = -1;
    double smallestDist = 9999999;
    
    for( int i=0; i<mCurrentObject.numSprites; i++ ) {
        
        double dist = distance( pos, mCurrentObject.spritePos[i] );
        
        if( dist < smallestDist ) {
            mPickedObjectLayer = i;
            
            smallestDist = dist;
            }
        }
    

    if( smallestDist > 100 ) {
        // too far to count as a pick
        mPickedObjectLayer = -1;
        }
    
    
    if( mPickedObjectLayer != -1 ) {
        
        mPickedMouseOffset = 
            sub( pos, mCurrentObject.spritePos[mPickedObjectLayer] );
        }
    
    }



void EditorObjectPage::pointerDrag( float inX, float inY ) {
    
    if( mPickedObjectLayer != -1 ) {
        doublePair pos = {inX, inY};
        

        mCurrentObject.spritePos[mPickedObjectLayer]
            = sub( pos, mPickedMouseOffset );
        }
    }



void EditorObjectPage::pointerUp( float inX, float inY ) {
    }



void EditorObjectPage::specialKeyDown( int inKeyCode ) {
    if( mPickedObjectLayer == -1 ) {
        return;
        }
    
    switch( inKeyCode ) {
        case MG_KEY_LEFT:
            mCurrentObject.spritePos[mPickedObjectLayer].x -= 1;
            break;
        case MG_KEY_RIGHT:
            mCurrentObject.spritePos[mPickedObjectLayer].x += 1;
            break;
        case MG_KEY_DOWN:
            mCurrentObject.spritePos[mPickedObjectLayer].y -= 1;
            break;
        case MG_KEY_UP:
            mCurrentObject.spritePos[mPickedObjectLayer].y += 1;
            break;
        }
    
            
    }
