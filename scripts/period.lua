FinishCondition = {}
function victoryTest()
  
  return false
end
production ={}
function passDay()
  	for v in planet.allDeposit do 
      if v.mine then
        if production[v.name]==nil then production[v.name]=0 end
        production[v.name]=production[v.name]+1
        if production[v.name]>=7 then
          production[v.name]=0
          
          coreManager:addresource(v:generate())
          end
        
        end
      
    end
 
 
end
function passWeek()
  
 
 
end
function buildbuilding(target)
	if FinishCondition["teloprter"]~=nil then
		target:addUpgrade("autosell")
	end
  return false
end
function buildunit(target)
	if FinishCondition["militarycovernor"]~=nil then
		target:setChar("Manpower",target:getChar("Manpower")+20)
		target:setChar("Firepower",target:getChar("Firepower")+20)
		target:setChar("Deployment capabilities",target:getChar("Deployment capabilities")+20)
	
	end
end

function countryWeek(target)
	if target:getchar("Propocadaocupied")==1 then
		t = actionMap["propogandaweekly"]
		for k,v in pairs(t) do 
		if v~=0 then
			target:changestate(k,v)
		end
	end
	end
end