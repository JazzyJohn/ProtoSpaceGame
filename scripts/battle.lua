function doBattle(battle)
	local i,supplies,start=0,0,battle.suppliedAmount
	local supplied =battle.isSupplied;
	local ismess ,endBattle=false,false;
	for v in battle.playerUnit do
		if v.name=="Messia" then ismess = true end
		if  v:hasUpgrade("supplyunit")  then
			supplied=true
			start=start+v:getChar("SupplieAdd")
		end
	end
	if ismess then 
		return battle.battleField:fanatism(10)
		
	end
	supplies =start/battle.playerAmount
	if  supplied then
		for v in battle.playerUnit do
		v:addSupplie(supplies)
		end
	end
	if battle.npcAmount==0 then
		local effectivefirepower=0;
		for v in battle.playerUnit do
			if not v:lowerHp(battle.partisansPower) then
				battle:deadUnit(0,i)
			end
			if v:hasUpgrade("fastgover") then
				for neib in battle.battleField.neibor do
					neib:dorecon()

				end
			end
			effectivefirepower =v:getFirePower()
			
			if supplied==false and v:hasUpgrade("officer") then
				effectivefirepower= effectivefirepower*2
			end
			
			if battle.playerAction==1 then
				battle.partisansHP = battle.partisansHP -effectivefirepower*v:getChar("Partisan Firepower")
				endBattle=battle.partisansHP<0
			elseif battle.playerAction==2 then
				endBattle=endBattle or battle.battleField:pillage(effectivefirepower*v:getChar("Pillage Firepower"))
			elseif battle.playerAction==3 then 
				endBattle=endBattle or battle.battleField:destroying(effectivefirepower*v:getChar("Destroy Firepower"))               
			end
			i= i+1
		end
	else 
		local unit=0
		for v in battle.playerUnit do
			unit= battle:getNpcUnit(i)
			if not  unit:lowerHp(v:getFirePower())then   battle:deadUnit(1,i) end
			i=i+1
			if i>=battle.npcAmount then i=0 end
		end
		i=0
		for v in battle.npcUnit do
			unit= battle:getPlayerUnit(i)
			if not  unit:lowerHp(v:getFirePower())then   battle:deadUnit(0,i) end
			i=i+1
			if i>=battle.playerAmount then i=0 end
		end  
		endBattle = false
	end
	return endBattle

end