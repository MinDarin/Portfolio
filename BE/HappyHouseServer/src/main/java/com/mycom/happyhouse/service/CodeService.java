package com.mycom.happyhouse.service;

import java.util.List;

import com.mycom.happyhouse.dto.CodeDto;

public interface CodeService {

	public List<CodeDto> getList(int groupCode);
}
