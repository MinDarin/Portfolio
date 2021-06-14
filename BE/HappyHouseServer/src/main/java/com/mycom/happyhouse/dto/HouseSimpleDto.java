package com.mycom.happyhouse.dto;

public class HouseSimpleDto {
	private String AptName;
	private String lat;
	private String lng;
	private String address;
	public String getAptName() {
		return AptName;
	}
	public void setAptName(String aptName) {
		AptName = aptName;
	}
	public String getLat() {
		return lat;
	}
	public void setLat(String lat) {
		this.lat = lat;
	}
	public String getLng() {
		return lng;
	}
	public void setLng(String lng) {
		this.lng = lng;
	}
	public String getAddress() {
		return address;
	}
	public void setAddress(String address) {
		this.address = address;
	}
	@Override
	public String toString() {
		return "HouseSimpleDto [AptName=" + AptName + ", lat=" + lat + ", lng=" + lng + ", address=" + address + "]";
	}	
	
}
