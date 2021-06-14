package com.mycom.happyhouse.service;

import java.util.HashMap;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.mycom.happyhouse.dao.HouseDao;
import com.mycom.happyhouse.dto.HouseDealDto;
import com.mycom.happyhouse.dto.HouseDto;
import com.mycom.happyhouse.dto.HouseResultDto;
import com.mycom.happyhouse.dto.HouseSimpleDto;

@Service
public class HouseServiceImpl implements HouseService {

	@Autowired
	HouseDao houseDao;
	
	public HouseResultDto getTotalInfo(String key, String word, int limit, int offset){
		HouseResultDto result = new HouseResultDto();
		result.setResultList(getInfo(key,word,limit,offset));
		result.setCount(getTotal(key,word));
		return result;
	}
	public List<HouseDto> getInfo(String key, String word, int limit, int offset){
		HashMap<String,Object> command = new HashMap();
		System.out.println("limit and offset"+limit+"/"+offset);
		command.put("key",key);
		command.put("word",word);
		command.put("limit",limit);
		command.put("offset",offset);	
		return houseDao.getInfo(command);
	} 
	public int getTotal(String key, String word){
		HashMap<String,String> command = new HashMap();
		command.put("key", key);
		command.put("word",word);
		return houseDao.getTotal(command);
	}
	
	public List<HouseSimpleDto> getAll(String key, String word){
		HashMap<String,Object> command = new HashMap();
		command.put("key",key);
		command.put("word",word);
		return houseDao.getAll(command);
	}
	
	public List<HouseDealDto> getDealInfo(int dealNo) {
		return houseDao.getDealInfo(dealNo);
	}
}
