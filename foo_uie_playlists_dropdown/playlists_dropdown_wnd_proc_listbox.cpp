#include "component.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Playlists Dropdown class implementation: ListBox window procedure
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

t_size ListBox_GetItemFromCoords(HWND wnd, LPARAM lp) {
	RECT  rc;
	POINT pt = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
	GetClientRect(wnd, &rc);
	return PtInRect(&rc, pt) ? ListBox_GetTopIndex(wnd) + pt.y / ListBox_GetItemHeight(wnd, 0) : pfc_infinite;
}

LRESULT CALLBACK playlists_dropdown::ListBoxWndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
		case WM_MBUTTONDOWN:
			{
				POINT pt;
				GetCursorPos(&pt);
				t_size idx = ListBox_GetItemFromCoords(wnd, lp);
				if (idx != pfc_infinite) {
					//m_block_capture = true;
					//if (DragDetect(m_hWndListBox, pt)) {
						metadb_handle_list items;
						static_api_ptr_t<playlist_manager>()->playlist_get_all_items(idx, items);

						// Create an IDataObject that contains the dragged track.
						static_api_ptr_t<playlist_incoming_item_filter> piif;
						// create_dataobject_ex() returns a smart pointer unlike create_dataobject()
						// which returns a raw COM pointer. The less chance we have to accidentally
						// get the reference counting wrong, the better.
						pfc::com_ptr_t<IDataObject> pDataObject = piif->create_dataobject_ex(items);

						// Create an IDropSource.
						// The constructor of IDropSource_tutorial1 is hidden by design; we use the
						// provided factory method which returns a smart pointer.
						pfc::com_ptr_t<IDropSource> pDropSource = IDropSource_tutorial1::g_create(m_hWndListBox);

						DWORD effect;
						// Perform drag&drop operation.

						DoDragDrop(pDataObject.get_ptr(), pDropSource.get_ptr(), DROPEFFECT_COPY, &effect);
						//m_block_capture = false;
					//}
					//m_block_capture = false;
					//SetFocus(m_hWndListBox);
					//SetCapture(m_hWndListBox);
					//return 0;
				}
			}
			break;

		case WM_CAPTURECHANGED:
			if (m_block_capture) {
				return 0;
			}
			break;

		case WM_CONTEXTMENU:
			{
				POINT pt;// = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
				GetCursorPos(&pt);
				POINT pt2(pt);
				ScreenToClient(wnd, &pt2);
				t_size idx = ListBox_GetItemFromCoords(wnd, MAKELPARAM(pt2.x, pt2.y));
				static_api_ptr_t<playlist_manager> pm;
				if (idx != pfc_infinite && idx < pm->get_playlist_count()) {
					m_block_capture = true;
					on_context_menu(wnd, idx, pt);
					//return 0;
				}
			}
			break;

		case WM_TIMER:
			if (wp == IDT_AUTOSWITCH) {
				KillTimer(wnd, IDT_AUTOSWITCH);
				if (m_dropping_idx != pfc_infinite) {
					static_api_ptr_t<playlist_manager>()->set_active_playlist(m_dropping_idx);
				}
			}
			break;

		case MSG_DRAG_OVER:
			{
				t_size idx = ListBox_GetItemFromCoords(wnd, lp);
				if (idx != pfc_infinite && idx != m_dropping_idx) {
					m_dropping_idx = idx;
					KillTimer(wnd, IDT_AUTOSWITCH);
					if (cfg::dd_autoswitch) {
						UINT_PTR timer = SetTimer(wnd, (UINT_PTR) IDT_AUTOSWITCH, cfg::dd_autoswitch_delay, NULL);
					}
				}
			}
			break;

		case MSG_DRAG_ENTER:
			m_dropping_idx = pfc_infinite;
			break;

		case MSG_DRAG_LEAVE:
			KillTimer(wnd, IDT_AUTOSWITCH);
			m_dropping_idx = pfc_infinite;
			ComboBox_ShowDropdown(m_hWnd, false);
			break;

		case MSG_DROP_FILES_EX:
			{
				KillTimer(wnd, IDT_AUTOSWITCH);
				POINT pt = { GET_X_LPARAM(lp), GET_Y_LPARAM(lp) };
				ClientToScreen(wnd, &pt);
				m_block_capture = true;
				on_dd_context_menu(wnd, ListBox_GetItemFromCoords(wnd, lp), pt, (IDataObject *) wp);
				ComboBox_ShowDropdown(m_hWnd, false);
				//ReleaseCapture();
			}
			break;

		case MSG_DROP_FILES:
			{
				KillTimer(wnd, IDT_AUTOSWITCH);
				m_dropping_idx = pfc_infinite;
				t_size idx = ListBox_GetItemFromCoords(wnd, lp);
				IDataObject * pDataObject = (IDataObject *) wp;
				service_ptr_t<process_locations_notify_impl> p_notify = new service_impl_t<process_locations_notify_impl>(idx);
				static_api_ptr_t<playlist_incoming_item_filter_v2>()->process_dropped_files_async(pDataObject, playlist_incoming_item_filter_v2::op_flag_delay_ui, core_api::get_main_window(), p_notify);
				ComboBox_ShowDropdown(m_hWnd, false);
				//ReleaseCapture();
			}
			break;

		/*case WM_MBUTTONDOWN:
			if (cfg::mbutton_reorder) {
				t_size idx = ListBox_GetItemFromCoords(wnd, lp);
				if (idx != pfc_infinite) {
					m_reordering = true;
					m_reordering_idx = idx;
					SetCursor(LoadCursor(NULL, IDC_HAND));
				}
			}
			break;*/

		case WM_MBUTTONUP:
			if (m_reordering) {
				m_reordering = false;
				m_reordering_idx = pfc_infinite;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
			break;

		case WM_MOUSEWHEEL:
			// ??? NOT WORKING HERE ???
			break;

		case WM_MOUSEMOVE:
			if (m_reordering) {
				t_size idx = ListBox_GetItemFromCoords(wnd, lp);
				if (idx >= 0 && m_reordering_idx >= 0 && idx != m_reordering_idx) {
					static_api_ptr_t< playlist_manager > pm;
					if (m_reordering_idx < pm->get_playlist_count() && idx < pm->get_playlist_count()) {
						order_helper order(pm->get_playlist_count());
						order.swap(m_reordering_idx, idx);
						pm->reorder(order.get_ptr(), pm->get_playlist_count());
						m_reordering_idx = idx;
					}
				}
			}
			break;
	}

	return CallWindowProc(OldListBoxProc, wnd, msg, wp, lp);
}
