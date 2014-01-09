#include "TestGame.h" 
#include "ClickHelper.h" 
#include <OgreRoot.h>

#include <OgreFont.h>
#include <OgreFontManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreManualObject.h>
const int TILESIZE =2;




Ogre::Vector3 calculateActualPointFromCenter(Ogre::Vector2 &center, const Ogre::Vector2& position){
	return Ogre::Vector3(position.x-center.x,position.y-center.y,0);

};

void TestGame::DrawPlanetMap(Logic::PlanetJustGenerateEvent *evnt){

	std::map<Logic::BaseCountry*,std::vector<Logic::CountryTile>>::iterator CntrItr;
	std::vector<Logic::CountryTile>::iterator TileItr;
	int tilesize=TILESIZE;
	bool why;
	Ogre::Vector3 tempVect;
	int maxX = (int)Logic::BaseAsset::GetSigleton("MAP_X"),maxY=(int)Logic::BaseAsset::GetSigleton("MAP_Y");
	Ogre::SceneNode* node,*capitalNode,*mapnode =mSceneMgr->getRootSceneNode()->createChildSceneNode("Map");
	mapnode->translate(-maxX/2*tilesize,-maxY/2*tilesize,0);

	Ogre::Entity* entNinja = 0;

	
	Ogre::FontPtr font = Ogre::FontManager::getSingleton().getResourceIterator().begin()->second; 

	// Make sure the texture is not WRITE_ONLY, we need to read the buffer to do the blending with the font (get the alpha for example)




	int j =0;
	for(CntrItr=evnt->allTiles.begin();CntrItr!=evnt->allTiles.end(); CntrItr++){

		Ogre::ManualObject* manual = mSceneMgr->createManualObject("CountryObject"+CntrItr->first->mName);

		Ogre::Texture* texture = Ogre::TextureManager::getSingleton().createManual("CountryText"+CntrItr->first->mName,"General",Ogre::TEX_TYPE_2D,512, 512, 0, Ogre::PF_FLOAT32_RGBA , Ogre::TU_DEFAULT).getPointer();
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("CountryMaterial"+CntrItr->first->mName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		//Draw the background to the new texture
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();

		// Lock the pixel buffer and get a pixel box
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		float *pDest = static_cast<float*>(pixelBox.data);
		Ogre::ColourValue *temp = &CntrItr->first->mColor;
		// Fill in some pixel data. This will give a semi-transparent blue,
		// but this is of course dependent on the chosen pixel format.
		
		for (size_t k = 0; k < 512; k++)
			for(size_t q = 0; q < 512; q++)
			{
				*pDest++ =temp->b; // B
				*pDest++ = temp->g;
				*pDest++ =temp->r;
				*pDest++ = 0; // A*/
				
				/* *pDest++ = 255; // B
        *pDest++ =   0; // G
        *pDest++ =   0; // R
        *pDest++ = 127; // A*/
			}

			// Unlock the pixel buffer
			pixelBuffer->unlock();






			///WriteToTexture(Logic::LogicStd::IntToString(j),Ogre::TexturePtr(texture),Ogre::Image::Box(25,275,370,500),font,Ogre::ColourValue(1.0,1.0,1.0,1.0),'c');
			 mat->getTechnique(0)->getPass(0)->createTextureUnitState("CountryText"+CntrItr->first->mName);
			// mat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
			j++;
			// specify the material (by name) and rendering type
			manual->begin("CountryMaterial"+CntrItr->first->mName, Ogre::RenderOperation::OT_TRIANGLE_LIST);
			int i=0;
			for(TileItr= CntrItr->second.begin();TileItr!= CntrItr->second.end();TileItr++){
				
				tempVect = calculateActualPointFromCenter(CntrItr->first->mCapital.mPosition,(*TileItr).mPosition);
				tempVect =tempVect*tilesize;
				manual->position(tempVect.x-tilesize/2,tempVect.y+tilesize/2,tempVect.z);
				//manual->colour(CntrItr->first->mColor);
				manual->position(tempVect.x+tilesize/2,tempVect.y+tilesize/2,tempVect.z);
				//manual->colour(CntrItr->first->mColor);
				manual->position(tempVect.x+tilesize/2,tempVect.y-tilesize/2,tempVect.z);
				//manual->colour(CntrItr->first->mColor);
				manual->position(tempVect.x-tilesize/2,tempVect.y-tilesize/2,tempVect.z);
				//manual->colour(CntrItr->first->mColor);
				manual->triangle(i*4+0,i*4+2,i*4+1);
				manual->triangle(i*4+0,i*4+3,i*4+2);

				//manual->quad(i*4+0,i*4+2,i*4+3,i*4+1);
				i++;
			}


			// tell Ogre, your definition has finished
			manual->end();

			// add ManualObject to the RootSceneNode (so it will be visible)
			node =mapnode->createChildSceneNode();
			entNinja= mSceneMgr->createEntity("Capital/"+CntrItr->first->mName, "capital.mesh");
			entNinja->setCastShadows(true);
			entNinja->setMaterialName("CountryMaterial"+CntrItr->first->mName);
			Control::ClickHelper* helpr = 	new Control::ClickHelper(Logic::CHT_COUNTRY);
			helpr->target = CntrItr->first;
			entNinja->setUserAny(Ogre::Any(helpr));
			CntrItr->first->mNode = node;
			capitalNode = node->createChildSceneNode();
			capitalNode->attachObject(entNinja);
			capitalNode->pitch(Ogre::Degree(90));
			node->attachObject(manual);
			// I move the SceneNode so that it is visible to the camera.
			tempVect = 	Ogre::Vector3(CntrItr->first->mCapital.mPosition.x*tilesize,CntrItr->first->mCapital.mPosition.y*tilesize,-10);
			node->translate(tempVect);


	} 
	mapnode->pitch(Ogre::Degree(-45));


}
void TestGame::DrawNewCore(Logic::CoreBuiltEvnt *evnt){

	Ogre::Entity* drop= mSceneMgr->createEntity("Mesh"+evnt->building->mSysName, "BaseDropNew.mesh");
	drop->setCastShadows(true);
	const Ogre::AxisAlignedBox test =drop->getBoundingBox();
	Control::ClickHelper* helpr = 	new Control::ClickHelper(Logic::CHT_BUILDING);
	helpr->target = evnt->building;
	drop->setUserAny(Ogre::Any(helpr));
	Ogre::SceneNode *coreNode = evnt->country->mNode->createChildSceneNode();
	Ogre::Vector3 tempvect = calculateActualPointFromCenter(evnt->country->mCapital.mPosition,evnt->tile->mPosition);
	tempvect= tempvect*TILESIZE;
	coreNode->attachObject(drop);
	coreNode->translate(tempvect);
	coreNode->pitch(Ogre::Degree(90));
	coreNode->scale(0.5,0.5,0.5);
	Ogre::AnimationState* temp= drop->getAnimationState("drop");
	temp->setLoop(false);
	temp->setEnabled(true);
	mAllAnimation .insert(temp);
	Ogre::Entity* basePlane= mSceneMgr->createEntity("MeshBaseFloor"+evnt->building->mSysName, "BaseCloseLook.mesh");
	basePlane->setMaterialName("BaseCloseLook/Rockwall");

	Ogre::SceneNode *BaseDraw = evnt->country->mNode->createChildSceneNode(),*camspot;
	BaseDraw->attachObject(basePlane);
	BaseDraw->translate(2000*evnt->building->mSlot,0,0);
	camspot = BaseDraw->createChildSceneNode("CamPoint_"+evnt->building->mSysName);
	camspot->setPosition(0,45,45);
	camspot->lookAt(Ogre::Vector3(0,0,0), Ogre::Node::TS_PARENT);
	helpr = 	new Control::ClickHelper(Logic::CHT_EMPTYSPACE);
	helpr->target = evnt->building;
	basePlane->setUserAny(Ogre::Any(helpr));
}