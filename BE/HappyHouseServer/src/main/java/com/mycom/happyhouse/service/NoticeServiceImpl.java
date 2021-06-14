package com.mycom.happyhouse.service;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.UUID;

import org.apache.commons.io.FilenameUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.multipart.MultipartHttpServletRequest;

import com.mycom.happyhouse.dao.NoticeDao;
import com.mycom.happyhouse.dto.NoticeDto;
import com.mycom.happyhouse.dto.NoticeFileDto;
import com.mycom.happyhouse.dto.NoticeParamDto;
import com.mycom.happyhouse.dto.NoticeResultDto;

@Service
public class NoticeServiceImpl implements NoticeService {

	@Autowired
	NoticeDao dao;

	String uploadFolder = "upload";

//	String uploadPath = "F:" + File.separator + "SSAFY" + File.separator + "ssafy5_sts3_boot" 
//			+ File.separator + "SpringBootVueNoticeFileUploadMybatis" 
//			+ File.separator + "src" 
//			+ File.separator + "main"
//			+ File.separator + "resources"
//			+ File.separator + "static";

	@Value("${uploadPath}")
	String uploadPath;

	private static final int SUCCESS = 1;
	private static final int FAIL = -1;

	@Override
	@Transactional
	public NoticeResultDto noticeInsert(NoticeDto dto, MultipartHttpServletRequest request) {
		
		NoticeResultDto noticeResultDto = new NoticeResultDto();
		
		try {
			dao.noticeInsert(dto); // useGeneratedKeys="true" keyProperty="NoticeNo"

			// for transaction test
//			String s = null;
//			s.length();
			
			List<MultipartFile> fileList = request.getFiles("file");
	
			File uploadDir = new File(uploadPath + File.separator + uploadFolder);
			if (!uploadDir.exists()) uploadDir.mkdir();

			for (MultipartFile part : fileList) {

				int noticeNo = dto.getNoticeNo();
				
				String fileName = part.getOriginalFilename();
				
				//Random File Id
				UUID uuid = UUID.randomUUID();
				
				//file extension
				String extension = FilenameUtils.getExtension(fileName); // vs FilenameUtils.getBaseName()
			
				String savingFileName = uuid + "." + extension;
			
				File destFile = new File(uploadPath + File.separator + uploadFolder + File.separator + savingFileName);
				
				System.out.println(uploadPath + File.separator + uploadFolder + File.separator + savingFileName);
				part.transferTo(destFile);
		    
			    // Table Insert
			    NoticeFileDto noticeFileDto = new NoticeFileDto();
			    noticeFileDto.setNoticeNo(noticeNo);
			    noticeFileDto.setFileName(fileName);
			    noticeFileDto.setFileSize(part.getSize());
				noticeFileDto.setFileContentType(part.getContentType());
				String noticeFileUrl = uploadFolder + "/" + savingFileName;
				noticeFileDto.setFileUrl(noticeFileUrl);
				
				dao.noticeFileInsert(noticeFileDto);
			}

			noticeResultDto.setResult(SUCCESS);
			
		}catch(IOException e) {
			e.printStackTrace();
			noticeResultDto.setResult(FAIL);
		}
		return noticeResultDto;
	}

	@Override
	@Transactional
	public NoticeResultDto noticeUpdate(NoticeDto dto, MultipartHttpServletRequest request){
		
		NoticeResultDto noticeResultDto = new NoticeResultDto();

		
		try {
			dao.noticeUpdate(dto);

			List<MultipartFile> fileList = request.getFiles("file");
			
			File uploadDir = new File(uploadPath + File.separator + uploadFolder);
			if (!uploadDir.exists()) uploadDir.mkdir();
			
			// 물리 파일 삭제, 첨부파일 여러개 고려
	    	List<String> fileUrlList = dao.noticeFileUrlDeleteList(dto.getNoticeNo());	
			for(String fileUrl : fileUrlList) {	
				File file = new File(uploadPath + File.separator, fileUrl);
				if(file.exists()) {
					file.delete();
				}
			}

	    	dao.noticeFileDelete(dto.getNoticeNo()); // ���̺� ���� ����
	    	
	    	
			for (MultipartFile part : fileList) {
				int noticeNo = dto.getNoticeNo();
				
				String fileName = part.getOriginalFilename();
				
				//Random File Id
				UUID uuid = UUID.randomUUID();
				
				//file extension
				String extension = FilenameUtils.getExtension(fileName); // vs FilenameUtils.getBaseName()
			
				String savingFileName = uuid + "." + extension;
			
				File destFile = new File(uploadPath + File.separator + uploadFolder + File.separator + savingFileName);
				
				System.out.println(uploadPath + File.separator + uploadFolder + File.separator + savingFileName);
				part.transferTo(destFile);
		    
			    // Table Insert
			    NoticeFileDto noticeFileDto = new NoticeFileDto();
			    noticeFileDto.setNoticeNo(noticeNo);
			    noticeFileDto.setFileName(fileName);
			    noticeFileDto.setFileSize(part.getSize());
				noticeFileDto.setFileContentType(part.getContentType());
				String noticeFileUrl = uploadFolder + "/" + savingFileName;
				noticeFileDto.setFileUrl(noticeFileUrl);
				
				dao.noticeFileInsert(noticeFileDto);
			}

			noticeResultDto.setResult(SUCCESS);
			
		}catch(IOException e) {
			e.printStackTrace();
			noticeResultDto.setResult(FAIL);
		}
		
		return noticeResultDto;
	}

	@Override
	@Transactional
	public NoticeResultDto noticeDelete(int noticeNo) {
		
		NoticeResultDto noticeResultDto = new NoticeResultDto();
		
		try {
			List<String> fileUrlList = dao.noticeFileUrlDeleteList(noticeNo);
			for(String fileUrl : fileUrlList) {
				File file = new File(uploadPath + File.separator, fileUrl);				
				if(file.exists()) {
					file.delete();
				}
			}
			
			dao.noticeFileDelete(noticeNo);
			dao.noticeReadCountDelete(noticeNo);
			dao.noticeDelete(noticeNo);		
			noticeResultDto.setResult(SUCCESS);
			
		}catch(Exception e) {
			e.printStackTrace();
			noticeResultDto.setResult(FAIL);
		}
		
		return noticeResultDto;
	}

	@Override
	@Transactional
	public NoticeResultDto noticeDetail(NoticeParamDto noticeParamDto) {
		
		NoticeResultDto noticeResultDto = new NoticeResultDto();
		
		try {
			int userReadCnt = dao.noticeUserReadCount(noticeParamDto);
			if( userReadCnt == 0 ) {
				dao.noticeUserReadInsert(noticeParamDto.getNoticeNo(), noticeParamDto.getUserSeq());
				dao.noticeReadCountUpdate(noticeParamDto.getNoticeNo());
			}
			
			NoticeDto dto = dao.noticeDetail(noticeParamDto);
			List<NoticeFileDto> fileList = dao.noticeDetailFileList(dto.getNoticeNo());
			
			dto.setFileList(fileList);
			noticeResultDto.setDto(dto);
			
			noticeResultDto.setResult(SUCCESS);
			
		}catch(Exception e) {
			e.printStackTrace();
			noticeResultDto.setResult(FAIL);
		}
		
		return noticeResultDto;
	}

	@Override
	public NoticeResultDto noticeList(NoticeParamDto noticeParamDto) {
		
		NoticeResultDto noticeResultDto = new NoticeResultDto();
		
		try {
			List<NoticeDto> list = dao.noticeList(noticeParamDto);
			int count = dao.noticeListTotalCount();			
			noticeResultDto.setList(list);
			noticeResultDto.setCount(count);
			noticeResultDto.setResult(SUCCESS);
			
		}catch(Exception e) {
			e.printStackTrace();
			noticeResultDto.setResult(FAIL);
		}
		
		return noticeResultDto;
	}

	@Override
	public NoticeResultDto noticeListSearchWord(NoticeParamDto noticeParamDto) {
		
		NoticeResultDto noticeResultDto = new NoticeResultDto();
		
		try {
			List<NoticeDto> list = dao.noticeListSearchWord(noticeParamDto);
			int count = dao.noticeListSearchWordTotalCount(noticeParamDto);
			
			noticeResultDto.setList(list);
			noticeResultDto.setCount(count);
			
			noticeResultDto.setResult(SUCCESS);
		
		}catch(Exception e) {
			e.printStackTrace();
			noticeResultDto.setResult(FAIL);
		}
		
		return noticeResultDto;
	}

}
