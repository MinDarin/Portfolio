package com.mycom.happyhouse.dto;

import java.util.List;

public class UserParamDto {
	private UserDto user;
	
	private List<Integer> storeCodeList;

	public UserDto getUser() {
		return user;
	}

	public void setUser(UserDto user) {
		this.user = user;
	}

	public List<Integer> getStoreCodeList() {
		return storeCodeList;
	}

	public void setStoreCodeList(List<Integer> storeCodeList) {
		this.storeCodeList = storeCodeList;
	}
	
	
}
