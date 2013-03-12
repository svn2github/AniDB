"use strict";

var tag_weight = {};
tag_weight[0]   = 'i_rate_';
tag_weight[100] = 'i_rate_halfstar';
tag_weight[200] = 'i_rate_verylow';
tag_weight[300] = 'i_rate_low';
tag_weight[400] = 'i_rate_med';
tag_weight[500] = 'i_rate_high';
tag_weight[600] = 'i_rate_veryhigh';

var tag_weight_rev = {};
tag_weight_rev['i_rate_']         = 0;
tag_weight_rev['i_rate_halfstar'] = 100;
tag_weight_rev['i_rate_verylow']  = 200;
tag_weight_rev['i_rate_low']      = 300;
tag_weight_rev['i_rate_med']      = 400;
tag_weight_rev['i_rate_high']     = 500;
tag_weight_rev['i_rate_veryhigh'] = 600;

function toggle_tag(tag, weight, is_parent) {
	var parent   = $('#tag-list div.tag#' + $(tag).attr('parentid'));
	if ($(tag).hasClass('is_abstract')) {
		if (parent.attr('id')) {
			toggle_tag(parent, weight, 1);
		}
		return;
	}

	var state = $('span.state span.i_icon', tag);
	if ($(tag).hasClass('has_weight')) {
		var icon  = $('span.weight span.i_icon', tag);
		for (var cls in tag_weight_rev) {
			if ($(icon).hasClass(cls)) {
				var cur_weight = (tag_weight_rev[cls] || 0);
				var old_weight = $('span.weight', tag).attr('value');
				if (weight === undefined) {
					weight = cur_weight + 100;
					if (!tag_weight[weight]) {
						weight = 0;
					}
				} else if (is_parent) {
					var min_weight = get_max_weight_of_childs($(tag).attr('id'));
					if (old_weight && old_weight >= min_weight) {
						if (cur_weight==600 && weight < cur_weight) {
							weight = old_weight;
						} else if (weight < old_weight) {
							weight = old_weight;
						}
					} else if (cur_weight==600 && weight < cur_weight) {
						weight = min_weight;
					} else if (weight < min_weight) {
						weight = min_weight;
					}
				}

				if (cur_weight == weight) {
					break;
				}

				var cls_new    = tag_weight[weight];
				$(icon).removeClass(cls);
				$(icon).addClass(cls_new);

				if ($(tag).hasClass('added')) {
					if (cls_new == 'i_rate_') {
						$(tag).addClass('delete');
						$(state).addClass('i_general_delete');
						$(tag).removeClass('added');
						$(state).removeClass('i_general_added');
						$(state).removeClass('i_general_edit');
						$(state).attr('title', 'to be deleted');
					} else if (old_weight != weight) {
						$(state).addClass('i_general_edit');
						$(state).removeClass('i_general_added');
						$(state).attr('title', 'weight was edited');
					} else {
						$(state).addClass('i_general_added');
						$(state).removeClass('i_general_edit');
						$(state).attr('title', 'was added previously with the same weight');
					}
				} else if ($(tag).hasClass('delete')) {
					if (old_weight != weight) {
						$(tag).addClass('added');
						$(state).addClass('i_general_edit');
						$(tag).removeClass('delete');
						$(state).removeClass('i_general_delete');
						$(state).attr('title', 'weight was edited');
					}
					else if (cls_new == 'i_rate_halfstar') {
						$(tag).addClass('added');
						$(state).addClass('i_general_added');
						$(tag).removeClass('delete');
						$(state).removeClass('i_general_delete');
						$(state).attr('title', 'was added previously with the same weight');
					}
				} else if ($(state).hasClass('i_general_new')) {
					if (cls_new == 'i_rate_') {
						$(state).removeClass('i_general_new');
					}
				} else {
					$(state).addClass('i_general_new');
					$(state).attr('title', 'to be added');
				}

				break;
			}
		}
	} else if ($(tag).hasClass('added')) {
		$(tag).addClass('delete');
		$(state).addClass('i_general_delete');
		$(tag).removeClass('added');
		$(state).removeClass('i_general_added');
		$(state).attr('title', 'to be deleted');
	} else if ($(tag).hasClass('delete')) {
		$(tag).addClass('added');
		$(state).addClass('i_general_added');
		$(tag).removeClass('delete');
		$(state).removeClass('i_general_delete');
		$(state).attr('title', 'was added previously');
	} else if ($(state).hasClass('i_general_new')) {
		$(state).removeClass('i_general_new');
	} else {
		$(state).addClass('i_general_new');
		$(state).attr('title', 'to be added');
	}

	if (parent.attr('id')) {
		toggle_tag(parent, weight, 1);
	}
}

function get_max_weight_of_childs (parentid) {
	var weights = $('#tag-list div.tag[parentid=' + parentid + ']').map( function() {
		for (var cls in tag_weight_rev) {
			if ($('span.weight span.i_icon', this).hasClass(cls)) {
				if (tag_weight_rev[cls] > 0) {
					return tag_weight_rev[cls];
				}
				break;
			}
		}
		return 0;
	}).get();

	return Math.max.apply(Math, weights);
}

function toggle_group(group) {
	if ($(group).hasClass('unset')) {
		$(group).removeClass('unset');
		$('#tag-list div.tag-column[groupid="' + $(group).attr("groupid") + '"]').removeClass('hide');
	} else {
		$(group).addClass('unset');
		$('#tag-list div.tag-column[groupid="' + $(group).attr("groupid") + '"]').addClass('hide');
	}
}

$(document).ready(function() {
	$("#tag-list div.tag").not('div.tag.is_abstract').click(function() {toggle_tag(this)});
	$("#tag-groups span.link").click(function() {toggle_group(this)});
});