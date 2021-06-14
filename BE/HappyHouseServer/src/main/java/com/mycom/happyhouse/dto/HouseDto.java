package com.mycom.happyhouse.dto;

public class HouseDto{
	private int infoNo;
	private int dealNo;
	private String maxAmount;
	private String minAmount;
	private String AptName;
	private String buildYear;
	private String jibun;
	private String lat;
	private String lng;
	private String dcode;
	private String img;
	private String address;
	public int getInfoNo() {
		return infoNo;
	}
	public void setInfoNo(int infoNo) {
		this.infoNo = infoNo;
	}
	public int getDealNo() {
		return dealNo;
	}
	public void setDealNo(int dealNo) {
		this.dealNo = dealNo;
	}
	public String getMaxAmount() {
		return maxAmount;
	}
	public void setMaxAmount(String maxAmount) {
		if(maxAmount == null) {
			this.maxAmount = "거래내역 없음";
		}
		else 
			this.maxAmount = maxAmount;
	}
	public String getMinAmount() {
		return minAmount;
	}
	public void setMinAmount(String minAmount) {
		if(minAmount == null) {
			this.minAmount = "거래내역 없음";
		}
		else 
			this.minAmount = minAmount;
	}
	public String getAptName() {
		return AptName;
	}
	public void setAptName(String aptName) {
		AptName = aptName;
	}
	public String getBuildYear() {
		return buildYear;
	}
	public void setBuildYear(String buildYear) {
		this.buildYear = buildYear;
	}
	public String getJibun() {
		return jibun;
	}
	public void setJibun(String jibun) {
		this.jibun = jibun;
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
	public String getDcode() {
		return dcode;
	}
	public void setDcode(String dcode) {
		this.dcode = dcode;
	}
	public String getImg() {
		return img;
	}
	public void setImg(String img) {
		if(img == null)
			this.img = "noimg.jpg";
		else
			this.img = img;
	}
	public String getAddress() {
		return address;
	}
	public void setAddress(String address) {
		this.address = address;
	}
}
