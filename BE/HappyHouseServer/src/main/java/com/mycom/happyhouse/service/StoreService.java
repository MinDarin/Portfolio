package com.mycom.happyhouse.service;

import com.mycom.happyhouse.dto.StoreParamDto;
import com.mycom.happyhouse.dto.StoreResultDto;

public interface StoreService {
	
	public StoreResultDto getList(StoreParamDto paramDto);
	
	public StoreResultDto getNearStoreList(StoreParamDto param);
}
