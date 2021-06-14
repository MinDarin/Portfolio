package com.mycom.happyhouse.dao;

import java.util.List;

import org.apache.ibatis.annotations.Mapper;

import com.mycom.happyhouse.dto.StoreDto;
import com.mycom.happyhouse.dto.StoreParamDto;

@Mapper
public interface StoreDao {

	public List<StoreDto> getList(StoreParamDto paramDto);
	
	public List<StoreDto> getNearStoreList(StoreParamDto pramDto);
}
