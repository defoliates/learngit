package net.dao;

import java.util.List;

import net.domain.Alarm;
import net.domain.DataFig;
import net.web.page.CriteriaObj;
import net.web.page.Page;

public interface AlarmDao {

	/**
	 * ���ݴ���� CriteriaBook ���󷵻ض�Ӧ�� Page ����
	 * @param cb
	 * @return
	 */
	public Page<Alarm> getPage(CriteriaObj cb);

	/**
	 * ���ݴ���� CriteriaBook ���󷵻����Ӧ�ļ�¼��
	 * @param cb
	 * @return
	 */
	public long getTotalAlarmNumber(CriteriaObj cb);

	/**
	 * ���ݴ���� CriteriaBook �� pageSize ���ص�ǰҳ��Ӧ�� List 
	 * @param cb
	 * @param pageNo
	 * @param pageSize
	 * @return
	 */
	public List<Alarm> getPageList(CriteriaObj cb,int pageSize);
	
	public List<DataFig> getDataSet(String start_time,String end_time);
}
