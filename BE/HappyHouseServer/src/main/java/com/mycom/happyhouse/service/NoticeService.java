package com.mycom.happyhouse.service;

import org.springframework.web.multipart.MultipartHttpServletRequest;

import com.mycom.happyhouse.dto.NoticeDto;
import com.mycom.happyhouse.dto.NoticeParamDto;
import com.mycom.happyhouse.dto.NoticeResultDto;


public interface NoticeService {
	
	public NoticeResultDto noticeDetail(NoticeParamDto noticeParamDto);
	
	public NoticeResultDto noticeDelete(int noticeNo);
	
	public NoticeResultDto noticeUpdate(NoticeDto dto, MultipartHttpServletRequest request);

	public NoticeResultDto noticeInsert(NoticeDto dto, MultipartHttpServletRequest request);

	public NoticeResultDto noticeList(NoticeParamDto noticeParamDto);

	public NoticeResultDto noticeListSearchWord(NoticeParamDto noticeParamDto);
	
}
