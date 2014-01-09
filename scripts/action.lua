actionMap ={	["First Contact mission"]={	
		["Envoirment"]=10,	
		["Alertness Lvl"]=100,
		["Population"]=0,
		["Economic"]=0	,
		["communication"]=0	,
		["Agression"]=0,
		["Trade"]=50,
		["IIF"]=0,
		["Worship"]=0,
		["Military Power"]=0
	},
	["Abduction missions"]={	
		["Envoirment"]=0,	
		["Alertness Lvl"]=5,
		["Population"]=-1,
		["Economic"]=0	,
		["communication"]=0	,
		["Agression"]=0,
		["Trade"]=0,
		["IIF"]=0,
		["Worship"]=0,
		["Military Power"]=0
	},
	["NormalMining"]={	
		["Envoirment"]=0,	
		["Alertness Lvl"]=0,
		["Population"]=0,
		["Economic"]=0	,
		["communication"]=0	,
		["Agression"]=0,
		["Trade"]=0,
		["IIF"]=-0.01,
		["Worship"]=0,
		["Military Power"]=0
	},
	["SecretMining"]={	
		["Envoirment"]=0,	
		["Alertness Lvl"]=1,
		["Population"]=0,
		["Economic"]=0	,
		["communication"]=0	,
		["Agression"]=0,
		["Trade"]=0,
		["IIF"]=0,
		["Worship"]=0,
		["Military Power"]=0
	},
	["ArtileryStrike"]={
		["Population"]=-1,
		["Economic"]=-1	,
		["IIF"]=-2,
	
	
	},
	["orbstrike"]={
		["Population"]=-50,
		["Economic"]=-50	,
		["communication"]=-50	,
		["Trade"]=-50,
		["IIF"]=-50,
		["Military Power"]=-50
	
  },
  ["demolition"]={
		["IIF"]=-10,
	 },
 ["propogandaweekly"]={
		["Economic"]=-10,
		["Military Power"]=-10,
		["Trade"]=-10,
	 }
}

actionSecretMap ={}
relationChange={["Abduction missions"]= -10}
actionMng= nil
--[[
country:setcahr(string,float)
country:changestate(string,float)
country:changeRelation(country,int);
country:ocupied
country:destroyed
country:atWar 

]]--
function ActionOccur(country,action,secret,succses,reason)
	if action =="MiningEffect" then
		if country:getchar("Alertness Lvl") >=100 then
			t = actionMap["NormalMining"]

		else
			t = actionMap["SecretMining"]

		end

		for k,v in pairs(t) do 
			if v~=0 then
				country:changestate(k,v)
			end
		end
	elseif action =="First Contact mission" then
		if country:getchar("Alertness Lvl") >=100 then
			return;
		end

		for k,v in pairs(actionMap[action]) do 
			if v~=0 then
				country:setchar(k,v)
			end
		end
		for v in planet.allCountry do 
			v:setchar("Alertness Lvl",100)
		end
		for v in country.allChar do 
			v:open()
		end

	else
		if action =="Abduction missions" then  
			country.recon= country.recon+1;
			for v in country.allChar do 
				v:deapason(country.recon,0)
			end
		end
		t= actionMap[action]
		if (secret==true and actionSecretMap[action]~=nil) then t= actionSecretMap[action]


		end
		if secret == true then
			country:changeRelation(reason,relationChange[action])
		end
		for k,v in pairs(t) do 
			if v~=0 then
				country:changestate(k,v)
			end
		end

	end

end
function canApplytoCountry(country,action)
	if action == "Peacekeeper mission" then
		return country:getchar("IIF")>=50
	end

	return true

end
function setActionMng(target)
	actionMng = target    
end
function returnPlanetType()
	return planet.type;

end
function add(obj)
	val =1
	obj:changestate(val)
	obj:changestate(val)
	obj:changestate(val)
end
