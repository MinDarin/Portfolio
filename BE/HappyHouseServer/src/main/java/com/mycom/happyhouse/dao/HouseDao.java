package com.mycom.happyhouse.dao;

import java.util.HashMap;
import java.util.List;

import org.apache.ibatis.annotations.Mapper;

import com.mycom.happyhouse.dto.HouseDealDto;
import com.mycom.happyhouse.dto.HouseDto;
import com.mycom.happyhouse.dto.HouseSimpleDto;

@Mapper
public interface HouseDao {
	public List<HouseDto> getInfo(HashMap<String,Object> command);
	public List<HouseSimpleDto> getAll(HashMap<String,Object> command);
	public int getTotal(HashMap<String,String> command);
	public List<HouseDealDto> getDealInfo(int dealNo);
}
