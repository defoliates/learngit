package net.dao;

import java.util.List;

import net.domain.DataFig;
import net.domain.NetFlow;
import net.web.page.CriteriaObj;
import net.web.page.Page;


public interface NetFlowDAO {


	/**
	 * ���ݴ���� CriteriaBook ���󷵻ض�Ӧ�� Page ����
	 * @param cb
	 * @return
	 */
	public abstract Page<NetFlow> getPage(CriteriaObj cb);

	/**
	 * ���ݴ���� CriteriaBook ���󷵻����Ӧ�ļ�¼��
	 * @param cb
	 * @return
	 */
	public abstract long getTotalNetFlowNumber(CriteriaObj cb);

	/**
	 * ���ݴ���� CriteriaBook �� pageSize ���ص�ǰҳ��Ӧ�� List 
	 * @param cb
	 * @param pageNo
	 * @param pageSize
	 * @return
	 */
	public abstract List<NetFlow> getPageList(CriteriaObj cb,int pageSize);

	public abstract List<DataFig> getTotalNetFlowData(String start_time,String end_time);
	
	@SuppressWarnings("rawtypes")
	public abstract List getTotalNetFlowData2(String start_time,String end_time);
	
	//public abstract List getTotalNetFlowData3(String start_time,String end_time);
}