package com.mycom.happyhouse;

public class PageNotPreparedException extends Exception{
	public PageNotPreparedException() {
		super("페이지 준비중입니다.");
	}
}
