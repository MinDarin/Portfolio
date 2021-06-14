package com.mycom.happyhouse.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.mycom.happyhouse.dao.StoreDao;
import com.mycom.happyhouse.dto.StoreParamDto;
import com.mycom.happyhouse.dto.StoreResultDto;

@Service
public class StoreServiceImpl implements StoreService{

	@Autowired
	StoreDao dao;
	
	private static final int SUCCESS = 1;
	private static final int FAIL = -1;
	
	@Override
	public StoreResultDto getList(StoreParamDto paramDto) {
		StoreResultDto result = new StoreResultDto();
		try {
			result.setList(dao.getList(paramDto));
			result.setResult(SUCCESS);
		}catch(Exception e) {
			e.printStackTrace();
			result.setResult(FAIL);
		}
		
		
		
		return result;
	}

	@Override
	public StoreResultDto getNearStoreList(StoreParamDto param) {
		StoreResultDto result = new StoreResultDto();
		
		try {
			result.setList(dao.getNearStoreList(param));
			result.setResult(SUCCESS);
			
		} catch(Exception e) {
			e.printStackTrace();
			result.setResult(FAIL);
		}
		
		
		return result;
	}
	
	
}
