package com.mycom.happyhouse.service;

import java.util.List;

import com.mycom.happyhouse.dto.HouseDealDto;
import com.mycom.happyhouse.dto.HouseDto;
import com.mycom.happyhouse.dto.HouseResultDto;
import com.mycom.happyhouse.dto.HouseSimpleDto;

public interface HouseService {
	public List<HouseDto> getInfo(String key, String word, int lmiit, int offset);
	public int getTotal(String key, String word);
	public List<HouseSimpleDto> getAll(String key, String word);
	public HouseResultDto getTotalInfo(String key, String word, int limit, int offset);
	public List<HouseDealDto> getDealInfo(int dealNo);
}
