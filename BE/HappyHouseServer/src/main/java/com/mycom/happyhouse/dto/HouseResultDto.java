package com.mycom.happyhouse.dto;

import java.util.List;

public class HouseResultDto {
	private List<HouseDto> resultList;
	private int count;
	public List<HouseDto> getResultList() {
		return resultList;
	}
	public void setResultList(List<HouseDto> resultList) {
		this.resultList = resultList;
	}
	public int getCount() {
		return count;
	}
	public void setCount(int count) {
		this.count = count;
	}
}
