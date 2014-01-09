#pragma once
#include <OgreRoot.h>
#include "BaseAccount.h"
namespace Logic{
	
	class BaseResource{
		public:
			BaseResource(void);
			BaseResource(Ogre::String name);
			~BaseResource(void);
			int mQty;
			Ogre::String mName; 
			virtual BaseResource & operator+= (int  &Qty);
			BaseResource &  operator-= (int  &Qty);
		
	};
	
	class BaseResourceObject{
		public:
			BaseResourceObject(void);
			BaseResourceObject(Ogre::String name);
			Ogre::String GetTooltip();
			void SetTooltip();
			~BaseResourceObject(void);
			std::map<BaseResource*,int> mResource;
			Ogre::String mName; 
			ResourceType mType;
			bool mRefinerable;
			int mTradePoint;
			void SetMap(std::map<BaseResource*,int> Resource);
			Ogre::String mTooltip;

	};
	struct ResourseSend{
		int amount;
		Ogre::String name;
		BaseResourceObject* object;
	};

	
	struct Production{
		bool refine;
		BaseResourceObject* from;
		std::map<BaseResourceObject*,int> recept;
		BaseResourceObject* result;
		float amount;		
	};
	struct SendProductionVar{
		SendProductionVar();
		Ogre::String from;
		Ogre::String to;
		int amount;
		Production* fromPtn;
		bool refine;
		std::map<Ogre::String,int> recept;
		bool active;
	};
	class BaseResourceManager
	{
		   friend class boost::serialization::access;
	public:
		LOADSAVE_FUNCTION_GENERATE()  
		BaseResourceManager(void);
		~BaseResourceManager(void);
		virtual BaseResource * addResource(Ogre::String name,int Qty=0);
		virtual void addResource(BaseResourceObject *Object);
		virtual void addResourceProduction(BaseResourceObject *source,BaseResourceObject *result,int amount);
		virtual void addResourceProduction(std::map<BaseResourceObject*,int> recept,BaseResourceObject *result,int amount);
		virtual void lowerResource(BaseResourceObject *Object);
		virtual void addResourceObject(BaseResourceObject *Object);
		void addResourceQueue(Ogre::String name);
		void takeResourceObject(Ogre::String name);
		bool hasResource(BaseResourceObject *Object);
		bool hasResource(BaseResourceObject *Object,int amount);
		void ReduceCredit(int cost);
		void AddCredit(int cost);
		void ShowObjectsAll(std::map<Ogre::String,int> &amount,std::map<Ogre::String,BaseResourceObject*> &links);
		void ShowObjectsAll(std::vector<ResourseSend> &allResourse);
		void ShowAvaibleProduction(std::list<SendProductionVar> &productionVar,Production*curProduction);
		bool GetQueueName(Ogre::String *name);
		Production * SayProduction(BaseResourceObject *Object);
		bool HasProduction(BaseResourceObject *Object,Production **prod);
		 std::map<Ogre::String,int> ShowResourse(void);
		std::vector<BaseResourceObject*> GetAllResourse(void);
		BaseResourceObject* GetResourceByName(Ogre::String);
		 BaseResource *  GetBaseResourceByName(Ogre::String name);
		std::vector<BaseResourceObject*> GetAllResourse(ResourceType type );
		std::vector<BaseResourceObject*> GetAllResourse(ResourceType type[],int size );
		BaseResourceObject * SayRefinery(BaseResourceObject *Object);
		BaseAccount *mAccount;
	private:
		std::map<Ogre::String,BaseResource*> mAllResource;
		std::map<Ogre::String,BaseResourceObject*> mAllObjects;
		std::map<ResourceType,std::vector<BaseResourceObject*>> mAllObjectSort;
		std::map<BaseResourceObject*,int> mAllObjectsCount;
		std::queue<Ogre::String> mObjectQueue;
		std::map<BaseResourceObject*,Production> mRefineryMap;
		std::vector<Production> mAllProduction;
	};
	
}

