package com.mycom.happyhouse.dto;

public class FavoriteDto {
	private int seq;
	private String si;
	private String gun;
	private String dong;
	private String dong_code;
	public String getDong_code() {
		return dong_code;
	}
	public void setDong_code(String dong_code) {
		this.dong_code = dong_code;
	}
	public int getSeq() {
		return seq;
	}
	public void setSeq(int seq) {
		this.seq = seq;
	}
	public String getSi() {
		return si;
	}
	public void setSi(String si) {
		this.si = si;
	}
	public String getGun() {
		return gun;
	}
	public void setGun(String gun) {
		this.gun = gun;
	}
	public String getDong() {
		return dong;
	}
	public void setDong(String dong) {
		this.dong = dong;
	}
}
