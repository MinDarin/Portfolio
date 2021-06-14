package com.mycom.happyhouse.dao;

import java.util.List;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import com.mycom.happyhouse.dto.NoticeDto;
import com.mycom.happyhouse.dto.NoticeFileDto;
import com.mycom.happyhouse.dto.NoticeParamDto;

@Mapper
public interface NoticeDao {
	public NoticeDto noticeDetail(NoticeParamDto NoticeParamDto);
	public List<NoticeFileDto> noticeDetailFileList(int noticeNo);
	
	// map - Dto
	public int noticeUserReadCount(NoticeParamDto NoticeParamDto); 
	
	// map - @param
	public int noticeUserReadInsert(
			@Param("noticeNo") int noticeNo, 
			@Param("userSeq") int userSeq ); 
	
	public int noticeReadCountUpdate(int noticeNo);
	
	
	public int noticeDelete(int noticeNo);	
	public int noticeFileDelete(int noticeNo);
	public List<String> noticeFileUrlDeleteList(int noticeNo);
	public int noticeReadCountDelete(int noticeNo);
	
	public int noticeInsert(NoticeDto dto);
	public int noticeFileInsert(NoticeFileDto dto);
	
	public List<NoticeDto> noticeList(NoticeParamDto NoticeParamDto);
	public int noticeListTotalCount();
	
	public List<NoticeDto> noticeListSearchWord(NoticeParamDto NoticeParamDto);
	public int noticeListSearchWordTotalCount(NoticeParamDto NoticeParamDto);
	

	public int noticeUpdate(NoticeDto dto);
}
