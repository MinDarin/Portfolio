package com.mycom.happyhouse.dto;

import java.time.LocalDateTime;

public class NoticeFileDto {
	private int noticeFileId;
	private int noticeNo;
	private String fileName;
	private long fileSize;
	private String fileContentType;
	private String fileUrl;
	private LocalDateTime regDt;
	public int getNoticeFileId() {
		return noticeFileId;
	}
	public void setNoticeFileId(int noticeFileId) {
		this.noticeFileId = noticeFileId;
	}
	public int getNoticeNo() {
		return noticeNo;
	}
	public void setNoticeNo(int noticeNo) {
		this.noticeNo = noticeNo;
	}
	public String getFileName() {
		return fileName;
	}
	public void setFileName(String fileName) {
		this.fileName = fileName;
	}
	public long getFileSize() {
		return fileSize;
	}
	public void setFileSize(long fileSize) {
		this.fileSize = fileSize;
	}
	public String getFileContentType() {
		return fileContentType;
	}
	public void setFileContentType(String fileContentType) {
		this.fileContentType = fileContentType;
	}
	public String getFileUrl() {
		return fileUrl;
	}
	public void setFileUrl(String fileUrl) {
		this.fileUrl = fileUrl;
	}
	public LocalDateTime getRegDt() {
		return regDt;
	}
	public void setRegDt(LocalDateTime regDt) {
		this.regDt = regDt;
	}
	
	
}