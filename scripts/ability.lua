abilityManager= nil
ability = {}  
uniqAbility ={}
ability["fastgover"] = function(target)
	if target:hasUpgrade("fastgover")~=true then

		target:addUpgrade("fastgover")
	end
	return true
end
ability["supplyunit"] = function(target)
	if  target:hasUpgrade("supplyunit")~=true then 
		target:setChar("SupplieAdd",10)
	end 
	return true;
end

ability["officer"] = function(target)
	if  target:hasUpgrade("officer")~=true then 
		target:addUpgrade("officer")
	end 
	return true;
end
ability["officer"] = function(target)
	if  target:hasUpgrade("officer")~=true then 
		target:addUpgrade("officer")
	end 
	return true;
end
ability["suppplydrop"] = function(target)
	if target.isSupplied == false then 
		target.isSupplied= true
		target.mSuppliedCoef  =2
	end 
	return true;
end
ability["punisher"] = function(target)
	if  target:hasUpgrade("punisher")~=true then 
		target:setChar("Partisan Firepower",2)
		target:addUpgrade("punisher")
	end 
	return true;
end
ability["pillage"] = function(target)
	if  target:hasUpgrade("punisher")~=true then 
		target:setChar("Pillage Firepower",2)
		target:addUpgrade("pillage")
	end 
	return true;
end
ability["heavy"] = function(target)
	if  target:hasUpgrade("heavy")~=true then 
		target:setChar("Firepower",target:getChar("Firepower")+30)
		target:setChar("Deployment capabilities",target:getChar("Deployment capabilities")-15)
		target:addUpgrade("heavy")
	end 
	return true;
end
ability["hocpital"] = function(target)
	if  target:hasUpgrade("hocpital")~=true then 
		target:setChar("Firepower",target:getChar("Firepower")-20)
		target:setChar("Manpower",target:getChar("Manpower")+20)
		target:setChar("SupplieCoef",target:getChar("SupplieCoef")*2)
		target:addUpgrade("hocpital")
	end 
	return true;
end
ability["heavyartillery"]= function(target) 
	if  target:hasUpgrade("heavyartillery")~=true then 
		target:addUpgrade("heavyartillery")
	end 
	return true;
end
ability["stealhtransport"]= function(target) 
	if  target:hasUpgrade("stealhtransport")~=true then 
		target:setChar("Manpower",target:getChar("Manpower")-20)
		target:setChar("Stealth",1)
		target:addUpgrade("stealhtransport")
	end 
	return true;
end
ability["orbstrike"]= function(target) 

	t = actionMap["orbstrike"]
	for k,v in pairs(t) do 
		if v~=0 then
			target:changestate(k,v)
		end
	end
	for v in planet.allCountry do 
		v:changestate("IIF",-30)

	end
	return true;
end
ability["orbitalcannon"]= function(target) 
	target.destroyed = true
	
	for v in planet.allCountry do 
		v:setchar("IIF",-100)

	end
	return true;
end
ability["localmarketanalyses"]= function(target)
	for v in target.demands do 
		v.open = true

	end
end
ability["mediabrodcast"] = function(target)
	if target:hasUpgrade("mediabrodcast")~=true then
		target.power= target.power *2
		target:addUpgrade("mediabrodcast");
	end
	return true;
end

ability["planetmarket"] = function(target)
	if uniqAbility["planetmarket"]== nil then
		uniqAbility["planetmarket"]= target
		assetMng:set("TRADE_COEF",assetMng.get("TRADE_COEF")*2)
		for v in planet.allCountry do 
			if 	v:getchar("Trade Post") then

				v:changestate("IIF",30)
			else
				v:changestate("IIF",-30)
			end
		end
	end
	return true;
end
ability["depofart"] = function(target)
	if target:hasUpgrade("depofart")~=true and target.core.country:getchar("IIF")>50 then

		target:addUpgrade("depofart")
	end
	return true;
end
ability["nativespec"] = function(target)
	target:setchar("IIF",0)
	return true;
end
ability["envcontrol"] = function(target)
	if target.core.country:getchar("Envoirment")==0 then
		target.core.country:setchar("Envoirment",1)
	end
	return true;
end
ability["orbitalpropoganda"] = function(target)
	if target.ocupied==false then
		target.ocupied= true
		target.setchar("Propocadaocupied",1)
		
		
	end
	return true;
end
ability["outmine"] = function(target)
	if target:hasUpgrade("outmine")~=true then
		target:addUpgrade("outmine");
	end
	return true;
end
ability["heavymine"] = function(target)
	if target.mine~=true then
		target.mine= true;
	end
	return true;
end
ability["autosell"] = function(target)
	if target:hasUpgrade("autosell")~=true then
		if uniqAbility["autosell"]~=nil then uniqAbility["autosell"].deleteUpgarde("autosell") end
		uniqAbility["autosell"]= target 
		target:addUpgrade("autosell")
	end
	return true;
end
ability["demolition"] = function(target)
	t = actionMap["demolition"]
	for k,v in pairs(t) do 
		if v~=0 then
			target:changestate(k,v)
		end
	end
	for v in target.neibor do 
		v:changestate("IIF",-30)

	end
	planet:newDeposit(target)

	return true;
end
ability["mobileref"] = function(target)
	if target:hasUpgrade("mobileref")~=true then
		if uniqAbility["mobileref"]~=nil then uniqAbility["mobileref"].deleteUpgarde("mobileref") end
		uniqAbility["mobileref"]= target 
		target:addUpgrade("mobileref")
	end
	return true;
end


function CallAbility (name,target)
	return ability[name](target)
end
function setAbilityMng(mng)
	abilityManager= mng
end




ability["Build Mine"] = function(target)
	target.mine = true
	return true
end

function unitDayPass(unit)
	if unit.onmove == false and unit:hasTarget() then
		target:hasUpgrade("heavyartillery")
		artilleryStrike(unit.country)
	end

end
function artilleryStrike(country)
	t = actionMap["ArtileryStrike"]
	for k,v in pairs(t) do 
		if v~=0 then
			country:changestate(k,v)
		end
	end
	for v in country.neibor do 
		v:changestate("IIF",-1)

	end
end
