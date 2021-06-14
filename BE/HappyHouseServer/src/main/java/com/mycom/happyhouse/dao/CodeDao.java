package com.mycom.happyhouse.dao;

import java.util.List;

import org.apache.ibatis.annotations.Mapper;

import com.mycom.happyhouse.dto.CodeDto;

@Mapper
public interface CodeDao {
	
	public List<CodeDto> getList(int groupCode);
}
