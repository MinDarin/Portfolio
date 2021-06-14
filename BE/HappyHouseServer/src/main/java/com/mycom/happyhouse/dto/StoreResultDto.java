package com.mycom.happyhouse.dto;

import java.util.List;

public class StoreResultDto {
	private int result;
	private List<StoreDto> list;

	
	public int getResult() {
		return result;
	}

	public void setResult(int result) {
		this.result = result;
	}

	public List<StoreDto> getList() {
		return list;
	}

	public void setList(List<StoreDto> list) {
		this.list = list;
	}
	
	
}
