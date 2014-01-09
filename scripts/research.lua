callBackMap ={}
callBackMap["advnmin"] = function ()
	abilityManager:OpenAbility("outmine")

end
callBackMap["encminscanner"] = function ()

	for v in planet.allCountry do 
		for k in v.allDeposit do
			k.open = true
		end

	end
end
callBackMap["minmark2"] = function ()

	for v in coreManager.blueprints do 
		if v.btype == 3 then
			v.cost = v.cost/2
			v:changeUpkeep(2)
		end
	end
	for v in coreManager.allbase do 
		for k in v.buildings do
			if k.btype == 3 then

				k:changeUpkeep(2)
			end
		end 

	end
end
callBackMap["heavyequip"] = function ()
	abilityManager:OpenAbility("heavymine")

end

callBackMap["dronebay"] = function ()
	for v in coreManager.allbase do 
		for k in v.buildings do
			if k.btype == 3 then

				k:SomeChange(0,1)
			end
		end 

	end

end
callBackMap["cloakfield"] = function ()
	actionMap["SecretMining"]["Alertness Lvl"]= actionMap["SecretMining"]["Alertness Lvl"]/2

end
callBackMap["orbmining"] = function ()
	for v in coreManager.blueprints do 
		if v.btype == 9 then
			v.open = true

		end
	end

end

callBackMap["demolising"] = function ()
	abilityManager:OpenAbility("demolising")
end 
callBackMap["advnlogistic"] = function ()
	abilityManager:OpenAbility("autosell")
end

callBackMap["heavydriling"] = function ()
	assetMng:set("MININGPRODUCTION_POWER",assetMng.get("MININGPRODUCTION_POWER")*2)

	for v in coreManager.allbase do 
		for k in v.buildings do
			if k.btype == 3 then

				k.power = k.power*2
			end
		end 

	end
end 
callBackMap["undergroundware"] = function ()
	for v in coreManager.allbase do 
		for k in v.buildings do
			if k.btype == 2 then

				k:SomeChange(1,2)
			end
		end 

	end
	for k in coreManager.main.buildings do
		if k.btype == 2 then

			k:SomeChange(1,2)
		end
	end 
	assetMng:set("WARHOUSESLOT_AMOUNT",assetMng.get("WARHOUSESLOT_AMOUNT")*2)
end
callBackMap["mobilerefinery"] = function ()
	abilityManager:OpenAbility("mobileref")
end
callBackMap["nanofabric"] = function ()
	for v in coreManager.allbase do 
		for k in v.buildings do
			if k.btype == 3 or  k.btype == 4 then

				k.power = 100
			end
		end 

	end
	FinishCondition["nanofabric"]=true
	assetMng:set("MININGPRODUCTION_POWER",100)
	assetMng:set("REFINERYPRODUCTION_POWER",100)
end
callBackMap["sunextractor"] = function ()
	FinishCondition["sunextractor"]=true
end
callBackMap["teloprter"] = function ()
	for v in coreManager.allbase do 
		for k in v.buildings do
			if k.btype == 3  then
				target:addUpgrade("autosell")
			end
		end 

	end
	FinishCondition["teloprter"]=true
end
callBackMap["hoveropen"] = function ()
	abilityManager:OpenAbility("fastgover")
end
callBackMap["loadingbay"] = function ()
	abilityManager:OpenAbility("supplyunit")
end
callBackMap["barackench"] = function ()
	abilityManager:OpenAbility("officer")
end

callBackMap["supplydrop"] = function ()
	abilityManager:OpenAbility("suppplydrop")
end
callBackMap["punishersq"] = function ()
	abilityManager:OpenAbility("punisher")
end
callBackMap["pillageteam"] = function ()
	abilityManager:OpenAbility("pillage")
end
callBackMap["heavyweapon"] = function ()
	abilityManager:OpenAbility("heavy")
end
callBackMap["mobilehospital"] = function ()
	abilityManager:OpenAbility("hocpital")
end
callBackMap["heavyartilery"] = function ()
	abilityManager:OpenAbility("heavyartillery")
end
callBackMap["stealth"] = function ()
	abilityManager:OpenAbility("stealhtransport")
end
callBackMap["orbitalbombard"] = function ()
	abilityManager:OpenAbility("orbstrike")
end
callBackMap["combatmech"] = function ()
	unitMng:open("CombatMech");

end
callBackMap["orbitalcannon"] = function ()
	FinishCondition["orbitalcannon"]=true
	abilityManager:OpenAbility("orbitalcannon")
end
callBackMap["militarycovernor"] = function ()
	for v in coreManager.allbase do 
		for k in v.buildings do
			if k.btype == 6 then

				k:SomeChange(2,20)
			end
		end 

	end
	for k in coreManager.main.buildings do
		if k.btype == 6 then

			k:SomeChange(2,20)
		end
	end 

	FinishCondition["militarycovernor"]=true

end
callBackMap["purifier"] = function ()
	FinishCondition["purifier"]=true
end
callBackMap["prepfirstcontact"] = function ()
	actionMap["First Contact mission"]["IIF"]= 100

end
callBackMap["mediabrodcast"] = function ()
	abilityManager:OpenAbility("mediabrodcast")

end
callBackMap["tradingfleetsupport"] = function ()
	abilityManager:OpenAbility("tradingfleetsupport")

end
callBackMap["localmarketanalise"]= function()
	abilityManager:OpenAbility("localmarketanalyses")
end
callBackMap["peackeeperopen"]= function()
	actionMng:open("Peace Keeper Mission")
end
callBackMap["merchantheadcuater"]= function()
	abilityManager:OpenAbility("planetmarket")
end
callBackMap["depart"]= function()
	abilityManager:OpenAbility("depofart")
end
callBackMap["educmission"]= function()
	actionMng:open("Peace Keeper Mission")
end
callBackMap["nativespecialists"]= function()
	actionMng:open("Native Specialists")
end
callBackMap["openenvcontrol"]= function()
	abilityManager:OpenAbility("envcontrol")
end
callBackMap["combinedengen"]= function()
	actionMng:open("Combined engineering")
end
callBackMap["medicaltech"]= function()
	actionMng:open("Hybrid medical tech")
end
callBackMap["atmospheric"] = function ()
	FinishCondition["atmospheric"]=true
end
callBackMap["orbitalpropoganda"] = function ()
	FinishCondition["orbitalpropoganda"]=true
	abilityManager:OpenAbility("orbitalpropoganda")
end
function ResearchFinish(research)
	if callBakMap[research.name]~=nil then
		callBakMap[research.name]()
	end

end